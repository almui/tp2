#include "VRPLIBReader.h"
#include <iostream>
#include <vector>
#include <cmath>
#include "Utils.h"
using namespace std;



// Realiza una búsqueda local tipo "relocate" para mejorar el costo total de las rutas
void relocate(vector<vector<int>>& rutas, const VRPLIBReader& instancia) {
    const vector<int>& demandas = instancia.getDemands(); // Demanda por cliente
    const vector<vector<double>>& matriz_distancias = instancia.getDistanceMatrix(); // Matriz de distancias
    int capacidad = instancia.getCapacity(); // Capacidad máxima del vehículo

    bool hay_mejora = true; // Para controlar si hubo mejora

    // Repetimos hasta que ya no se encuentren mejoras posibles
    while (hay_mejora) {
        hay_mejora = false;

        // Guardamos el costo y rutas actuales como mejores por ahora
        double mejor_costo = calcularCostoTotal(rutas, matriz_distancias);
        vector<vector<int>> mejor_rutas = rutas;

        // Recorremos todos los pares de rutas distintas
        for (int r1 = 0; r1 < rutas.size(); ++r1) {
            for (int r2 = 0; r2 < rutas.size(); ++r2) {
                if (r1 == r2) continue; // No se permite mover dentro de la misma ruta

                // Recorremos todos los clientes de r1 (menos depósito en extremos)
                for (int i = 1; i < rutas[r1].size() - 1; ++i) {
                    int cliente = rutas[r1][i];
                    int demanda_cliente = demandas[cliente];

                    // Calculamos la demanda actual de r2
                    int demanda_total_r2 = 0;
                    for (int k = 1; k < rutas[r2].size() - 1; ++k) {
                        demanda_total_r2 += demandas[rutas[r2][k]];
                    }

                    // Verificamos que mover el cliente no exceda la capacidad
                    if (demanda_total_r2 + demanda_cliente > capacidad) continue;

                    // Probamos insertar el cliente en todas las posiciones posibles de r2
                    for (int j = 1; j < rutas[r2].size(); ++j) {
                        // Hacemos una copia para no modificar las originales
                        vector<vector<int>> copia_rutas = rutas;
                        vector<int>& nueva_r1 = copia_rutas[r1];
                        vector<int>& nueva_r2 = copia_rutas[r2];

                        // Quitamos el cliente de r1
                        nueva_r1.erase(nueva_r1.begin() + i);

                        // Insertamos el cliente en la posición j de r2
                        nueva_r2.insert(nueva_r2.begin() + j, cliente);

                        // Calculamos el nuevo costo con este cambio
                        double nuevo_costo = calcularCostoTotal(copia_rutas, matriz_distancias);

                        // Si es mejor, lo guardamos como la nueva mejor solución
                        if (nuevo_costo < mejor_costo) {
                            mejor_costo = nuevo_costo;
                            mejor_rutas = copia_rutas;
                            hay_mejora = true;
                        }
                    }
                }
            }
        }

        // Si encontramos una mejora, actualizamos las rutas
        if (hay_mejora) {
            rutas = mejor_rutas;
        }
    }
}