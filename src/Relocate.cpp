#include "VRPLIBReader.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

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

void relocate(vector<vector<int>>& rutas, const VRPLIBReader& instancia) {
    const vector<int>& demandas = instancia.getDemands();               // Vector con la demanda de cada cliente
    const vector<vector<double>>& matriz_distancias = instancia.getDistanceMatrix();  // Matriz de distancias
    int capacidad = instancia.getCapacity();                            // Capacidad del vehículo

    bool hay_mejora = true;           // Solo se pone a true si hay una mejora real

    // Se repite mientras se sigan encontrando mejoras
    while (hay_mejora) {
        hay_mejora = false;

        // Variables para almacenar la mejor solución encontrada
        double mejor_costo = calcularCostoTotal(rutas, matriz_distancias);
        vector<vector<int>> mejor_rutas = rutas;

        // Recorre cada par de rutas diferentes (r1 y r2)
        for (int r1 = 0; r1 < rutas.size(); ++r1) {
            for (int r2 = 0; r2 < rutas.size(); ++r2) {
                if (r1 == r2) continue;  // No se permite mover dentro de la misma ruta

                // Recorre los clientes de la ruta r1 (excluyendo el depósito)
                for (int i = 1; i < rutas[r1].size() - 1; ++i) {
                    int cliente = rutas[r1][i];
                    int demanda_cliente = demandas[cliente];

                    // Suma la demanda total actual de la ruta r2
                    int demanda_total_r2 = 0;
                    for (int k = 1; k < rutas[r2].size() - 1; ++k) {
                        demanda_total_r2 += demandas[rutas[r2][k]];
                    }

                    // Verifica si mover el cliente no excede la capacidad del vehículo
                    if (demanda_total_r2 + demanda_cliente > capacidad) continue;

                    // Intenta insertar el cliente en todas las posiciones válidas de r2
                    for (int j = 1; j < rutas[r2].size(); ++j) {
                        // Copia temporal de las rutas para probar el cambio
                        vector<vector<int>> copia_rutas = rutas;
                        vector<int>& nueva_r1 = copia_rutas[r1];
                        vector<int>& nueva_r2 = copia_rutas[r2];

                        // Elimina el cliente de la ruta original
                        nueva_r1.erase(nueva_r1.begin() + i);

                        // Inserta el cliente en la nueva posición de la otra ruta
                        nueva_r2.insert(nueva_r2.begin() + j, cliente);

                        // Calcula el nuevo costo total
                        double nuevo_costo = calcularCostoTotal(copia_rutas, matriz_distancias);

                        // Si el nuevo costo mejora el anterior, guardamos esta configuración
                        if (nuevo_costo < mejor_costo) {
                            mejor_costo = nuevo_costo;
                            mejor_rutas = copia_rutas;
                            hay_mejora = true;
                        }
                    }
                }
            }
        }

        // Si se encontró una mejora, se actualiza la solución actual
        if (hay_mejora) {
            rutas = mejor_rutas;
        }
    }
}


