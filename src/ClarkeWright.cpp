#include "ClarkeWright.h"
#include "Solution.h"
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


// Clarke & Wright + llamada a relocate al final
Solution clarke_wright(const VRPLIBReader& instancia){

    Solution sol(instancia);
    
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
    for (int i = 1; i < dimension; ++i) {
        if (i == deposito) continue; // No tener en cuenta el depósito 
        rutas[i] = {deposito, i, deposito}; // Ruta con un solo cliente
        demanda_ruta[i] = demandas[i];      // Demanda correspondiente al cliente
    }

    // Vector para almacenar los ahorros calculados para posibles fusiones de rutas
    vector<Ahorro> ahorros;

    // Calcular los ahorros para todas los pares de clientes (i, j)
    for (int i = 1; i < dimension; ++i) {
        for (int j = i + 1; j < dimension; ++j) {
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
        // No hay fusiones CW que mejoren la solución.
        Solution sin_fusion(instancia);  // Creamos una Solution vacía
        for (int k = 1; k < dimension; ++k) {
            if (rutas[k].size() > 2) { // Solo rutas con clientes
                const vector<int>& ruta = rutas[k];
                int cliente_inicial = ruta[1];
                sin_fusion.addRuta(cliente_inicial);

                for (int i = 2; i < ruta.size() - 1; ++i) {
                    int atras = ruta[i - 1];
                    int actual = ruta[i];
                    int adelante = ruta[i + 1];
                    sin_fusion.addClient(actual, sin_fusion.getRutas().size() - 1, atras, adelante);
                }
            }
        }
        return sin_fusion;
    }
    
    // Iterar sobre los ahorros ordenados para fusionar rutas si es posible
    for (const Ahorro& a : ahorros) {
        int i = a.i; 
        int j = a.j; 

        // Índices de las rutas que contienen i y j
        int ruta_i = -1;
        int ruta_j = -1; 

        // Buscar las rutas a las que pertenecen i y j
        for (int k = 1; k < dimension; ++k) {
            if (!rutas[k].empty() && rutas[k][1] == i) {
                ruta_i = k; // i está al inicio de ruta k
            }
            if (!rutas[k].empty() && rutas[k][rutas[k].size() - 2] == j) {
                ruta_j = k; // j está al final de ruta k
            }
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

    // Añadimos rutas fusionadas finales a la solución
    for(int k = 1; k < dimension; k++) {
        if (!rutas[k].empty()) {
            const vector<int>& ruta = rutas[k];
            if (ruta.size() >= 3) {
                int cliente_inicial = ruta[1];
                sol.addRuta(cliente_inicial);

                for (int i = 2; i < ruta.size() - 1; i++) {
                    int atras = ruta[i - 1];
                    int actual = ruta[i];
                    int adelante = ruta[i + 1];
                    sol.addClient(actual, sol.getRutas().size() - 1, atras, adelante);
                }
            }
        }

    return sol;
}
}
