#include "Swap.h"
#include "Utils.h"

Solution route_swap(const Solution& solution_original) {
    Solution solution = solution_original;

    const VRPLIBReader& instancia = solution.getInstancia();
    const vector<int>& demandas = instancia.getDemands();
    int capacidad = instancia.getCapacity();

    bool hay_mejora = true;

    while (hay_mejora) {
        hay_mejora = false;
        double mejor_costo = calcularCostoTotal(solution);
        vector<vector<int>> rutas = solution.getRutas();

        bool mejora_encontrada = false;

        for (int r1 = 0; r1 < rutas.size() && !mejora_encontrada; ++r1) {
            for (int r2 = 0; r2 < rutas.size() && !mejora_encontrada; ++r2) {
                if (r1 == r2 && i == j) continue;

                for (int i = 1; i < rutas[r1].size() - 1 && !mejora_encontrada; ++i) {
                    for (int j = 1; j < rutas[r2].size() - 1 && !mejora_encontrada; ++j) {

                        int c1 = rutas[r1][i];
                        int c2 = rutas[r2][j];
                        int demanda_c1 = demandas[c1 + 1];
                        int demanda_c2 = demandas[c2 + 1];

                        int nueva_d1 = solution.getDemandas()[r1] - demanda_c1 + demanda_c2;
                        int nueva_d2 = solution.getDemandas()[r2] - demanda_c2 + demanda_c1;

                        if (nueva_d1 > capacidad || nueva_d2 > capacidad) continue;

                        Solution copia = solution;

                        copia.removeClient(r1, i);
                        copia.removeClient(r2, j);

                        // Adjust index if on the same route
                        if (r1 == r2 && i < j) j--;

                        copia.addClient(c2, r1, i);
                        copia.addClient(c1, r2, j);

                        double nuevo_costo = calcularCostoTotal(copia);

                        if (nuevo_costo < mejor_costo) {
                            solution = copia;
                            mejor_costo = nuevo_costo;
                            hay_mejora = true;
                            mejora_encontrada = true;
                        }
                    }
                }
            }
        }
    }

    return solution;
}
