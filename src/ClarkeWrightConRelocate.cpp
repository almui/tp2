#include "ClarkeWrightConRelocate.h"
#include "VRPLIBReader.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// Ordenar ahorros de mayor a menor
void ordenar_ahorros(vector<Ahorro>& ahorros) {
    for (int i = 0; i < ahorros.size() - 1; ++i) {
        for (int j = i + 1; j < ahorros.size(); ++j) {
            if (ahorros[j].valor > ahorros[i].valor) {
                Ahorro temp = ahorros[i];
                ahorros[i] = ahorros[j];
                ahorros[j] = temp;
            }
        }
    }
}

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


// Aplica el operador relocate para intentar reducir el costo total del conjunto de rutas
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



// Clarke & Wright + llamada a relocate al final
vector<vector<int>> clarke_wright(const VRPLIBReader& instancia) {
    // Obtener el ID del depósito de la instancia (normalmente nodo 0 o similar)
    int deposito = instancia.getDepotId();
    // Obtener la cantidad total de nodos (incluyendo depósito y clientes)
    int dimension = instancia.getDimension();
    // Obtener la capacidad máxima de los vehículos
    int capacidad = instancia.getCapacity();
    // Obtener la lista de demandas de los clientes
    const vector<int>& demandas = instancia.getDemands();
    // Obtener la matriz de distancias entre todos los nodos
    const auto& dist = instancia.getDistanceMatrix();

    // Inicializar un vector de rutas, una para cada nodo (de tamaño dimension+1)
    // Las rutas se almacenan como vectores de enteros (nodos)
    vector<vector<int>> rutas(dimension + 1);
    // Vector para almacenar la demanda acumulada de cada ruta
    vector<int> demanda_ruta(dimension + 1, 0);

    // Crear rutas iniciales triviales: cada cliente tiene su propia ruta [deposito, cliente, deposito]
    for (int i = 1; i <= dimension; ++i) {
        if (i == deposito) continue; // No tener en cuenta el depósito 
        rutas[i] = {deposito, i, deposito}; // Ruta con un solo cliente
        demanda_ruta[i] = demandas[i];      // Demanda correspondiente al cliente
    }

    // Vector para almacenar los ahorros calculados para posibles fusiones de rutas
    vector<Ahorro> ahorros;

    // Calcular los ahorros para todas los pares de clientes (i, j)
    for (int i = 1; i <= dimension; ++i) {
        for (int j = i + 1; j <= dimension; ++j) {
            // No considerar pares que involucren al depósito
            if (i == deposito || j == deposito) continue;
            // Calcular ahorro: costo actual - costo si se conecta directamente i-j
            double ahorro = dist[deposito][i] + dist[deposito][j] - dist[i][j];
            // Guardar el ahorro junto con los nodos involucrados
            ahorros.push_back({i, j, ahorro});
        }
    }

    // Ordenar los ahorros de mayor a menor para aplicar primero las fusiones más beneficiosas y que se consideren las que resultan un ahorro > 0
    ordenar_ahorros(ahorros);

    // Caso donde no hay ahorros en las posibles fusiones y es mejor no fusionar las rutas 
    if (!ahorros.empty() && ahorros[0].valor <= 0) {
        // No hay fusiones CW que mejoren la solución. Retornar rutas iniciales con relocate.
        vector<vector<int>> resultado_sin_cw_fusion;
        for (int k = 1; k <= dimension; ++k) {
            if (rutas[k].size() > 2) { // Solo rutas con clientes
                resultado_sin_cw_fusion.push_back(rutas[k]);
            }
        }
        relocate(resultado_sin_cw_fusion, instancia);
        return resultado_sin_cw_fusion;
    }
    
    // Iterar sobre los ahorros ordenados para fusionar rutas si es posible
    for (const Ahorro& a : ahorros) {
        int i = a.i; 
        int j = a.j; 

        // Índices de las rutas que contienen i y j
        int ruta_i = -1;
        int ruta_j = -1; 

        // Buscar las rutas a las que pertenecen i y j
        for (int k = 1; k <= dimension; ++k) {
            if (!rutas[k].empty() && rutas[k][1] == i) ruta_i = k; // i está al inicio de ruta k
            if (!rutas[k].empty() && rutas[k][rutas[k].size() - 2] == j) ruta_j = k; // j está al final de ruta k
        }

        // Solo fusionar si:
        // - Ambos nodos están en rutas diferentes
        // - No son rutas vacías
        if (ruta_i != -1 && ruta_j != -1 && ruta_i != ruta_j) {
            // Comprobar que la suma de demandas no exceda la capacidad máxima
            if (demanda_ruta[ruta_i] + demanda_ruta[ruta_j] <= capacidad) {
                vector<int>& r_i = rutas[ruta_i]; // Ruta de i
                vector<int>& r_j = rutas[ruta_j]; // Ruta de j

                r_j.pop_back(); // Remover el depósito final de la ruta j para fusionar

                // Añadir todos los nodos de r_i (excepto depósitos) al final de r_j
                for (int idx = 1; idx < r_i.size() - 1; ++idx) {
                    r_j.push_back(r_i[idx]);
                }
                r_j.push_back(deposito); // Añadir de nuevo el depósito al final

                // Actualizar la demanda acumulada de la ruta fusionada
                demanda_ruta[ruta_j] += demanda_ruta[ruta_i];
                rutas[ruta_i].clear(); // Vaciar la ruta i ya que fue fusionada
            }
        }
    }

    // Filtrar las rutas que quedaron vacías tras las fusiones
    vector<vector<int>> resultado;
    for (int k = 1; k <= dimension; ++k) {
        if (!rutas[k].empty()) resultado.push_back(rutas[k]);
    }

    // Aplicar la mejora local relocate sobre la solución generada
    relocate(resultado, instancia);

    return resultado;
}
