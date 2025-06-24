#include "Solution.h"
#include "VRPLIBReader.h"
#include "Utils.h"

Solution relocate(const Solution initial_sol) {
    Solution best_sol = initial_sol;
    VRPLIBReader instancia = best_sol.getInstancia();
    const vector<int>& demandas = instancia.getDemands();
    int capacidad = instancia.getCapacity();

    bool hay_mejora = true;

    while (hay_mejora) {
        hay_mejora = false;
        Solution mejor_sol = best_sol;
        double mejor_costo = 0.0;
        for (double d : mejor_sol.getDistancias()) mejor_costo += d;

        const vector<vector<int>>& rutas = best_sol.getRutas();

        for (int r1 = 0; r1 < rutas.size(); ++r1) {
            for (int r2 = 0; r2 < rutas.size(); ++r2) {
                if (r1 == r2) continue;

                const vector<int>& ruta1 = rutas[r1];
                const vector<int>& ruta2 = rutas[r2];

                for (int i = 1; i < ruta1.size() - 1; ++i) {
                    int cliente = ruta1[i];
                    int demanda_cliente = demandas[cliente];

                    int demanda_total_r2 = 0;
                    for (int k = 1; k < ruta2.size() - 1; ++k) {
                        demanda_total_r2 += demandas[ruta2[k]];
                    }

                    if (demanda_total_r2 + demanda_cliente > capacidad) continue;

                    for (int j = 1; j < ruta2.size(); ++j) {
                        Solution nueva_sol = best_sol;

                        nueva_sol.removeClient(r1, i);
                        nueva_sol.addClient(cliente, r2, j);

                        if (!nueva_sol.esValida(r1) || !nueva_sol.esValida(r2)) continue;

                        double nuevo_costo = 0.0;
                        for (double d : nueva_sol.getDistancias()) nuevo_costo += d;

                        if (nuevo_costo < mejor_costo) {
                            mejor_costo = nuevo_costo;
                            mejor_sol = nueva_sol;
                            hay_mejora = true;
                        }
                    }
                }
            }
        }

        if (hay_mejora) {
            best_sol = mejor_sol;
        }
    }

    return best_sol;
}
