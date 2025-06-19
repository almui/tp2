#include "Utils.h"
// Calcula el costo total de un conjunto de rutas
// El costo de una ruta es la suma de las distancias entre nodos consecutivos
double calcularCostoTotal(Solution sol) {
    double costo_total = 0.0;
    vector<vector<double>> matriz_distancias = sol.getDistanceMatrix();
    vector<vector<double>> rutas = sol.getRutas();
    // Recorre cada ruta
    for (const vector<int>& ruta : rutas) {
        // Suma las distancias entre pares consecutivos de nodos
        for (size_t i = 0; i < ruta.size() - 1; ++i) {
            int desde = ruta[i];
            int hacia = ruta[i + 1];
            costo_total += matriz_distancias[desde][hacia];
        }
    }

    return costo_total;
}

int calcularDemandaRuta(Solution sol, int indexRuta){
    int demanda = 0;
    vector<vector<int>> rutas = sol.getRutas();
    vector<int> miRuta = rutas[indexRuta];

    for (int i=0; i<miRuta.size(); i++){
        demanda+=sol.getDemandas(miRuta[i]);
    }
    return demanda;
}