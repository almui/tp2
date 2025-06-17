#include "Utils.h"
// Calcula el costo total de un conjunto de rutas
// El costo de una ruta es la suma de las distancias entre nodos consecutivos
double calcularCostoTotal(const vector<vector<int>>& rutas, const vector<vector<double>>& matriz_distancias) {
    double costo_total = 0.0;

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