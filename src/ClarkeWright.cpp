#include "ClarkeWright.h"
#include "VRPLIBReader.h"
#include "Solution.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

// Usar la estructura Ahorro definida en el header

// Función para encontrar en qué ruta está un nodo y en qué posición
pair<int, int> encontrar_nodo_en_rutas(const vector<vector<int>>& rutas, int nodo, int dimension) {
    for (int k = 1; k <= dimension; ++k) {
        if (rutas[k].size() > 2) { // Solo rutas con clientes
            for (int pos = 1; pos < rutas[k].size() - 1; ++pos) {
                if (rutas[k][pos] == nodo) {
                    return {k, pos}; // {índice_ruta, posición_en_ruta}
                }
            }
        }
    }
    return {-1, -1}; // No encontrado
}

// Función para verificar si dos nodos pueden fusionarse
bool pueden_fusionarse(const vector<vector<int>>& rutas, int i, int j, 
                      int ruta_i, int pos_i, int ruta_j, int pos_j) {
    if (ruta_i == ruta_j) return false; // Mismo ruta
    if (ruta_i == -1 || ruta_j == -1) return false; // Alguno no encontrado
    
    // Verificar que i esté al final de su ruta y j al inicio de la suya
    bool i_al_final = (pos_i == rutas[ruta_i].size() - 2);
    bool j_al_inicio = (pos_j == 1);
    
    // O que j esté al final de su ruta e i al inicio de la suya
    bool j_al_final = (pos_j == rutas[ruta_j].size() - 2);
    bool i_al_inicio = (pos_i == 1);
    
    return (i_al_final && j_al_inicio) || (j_al_final && i_al_inicio);
}

// Clarke & Wright mejorado
Solution clarke_wright(const VRPLIBReader& instancia) {
    // Obtener parámetros de la instancia
    int deposito = instancia.getDepotId();
    int dimension = instancia.getDimension();
    int capacidad = instancia.getCapacity();
    const vector<int>& demandas = instancia.getDemands();
    const auto& dist = instancia.getDistanceMatrix();

    // Inicializar rutas y demandas
    vector<vector<int>> rutas(dimension + 1);
    vector<int> demanda_ruta(dimension + 1, 0);

    // Crear rutas iniciales: cada cliente tiene su propia ruta
    for (int i = 1; i <= dimension; ++i) {
        if (i == deposito) continue;
        rutas[i] = {deposito, i, deposito};
        demanda_ruta[i] = demandas[i];
    }

    // Calcular ahorros para todos los pares de clientes
    vector<Ahorro> ahorros;
    for (int i = 1; i <= dimension; ++i) {
        for (int j = i + 1; j <= dimension; ++j) {
            if (i == deposito || j == deposito) continue;
            
            double ahorro = dist[deposito][i] + dist[deposito][j] - dist[i][j];
            if (ahorro > 0) { // Solo considerar ahorros positivos
                Ahorro a;
                a.i = i;
                a.j = j;
                a.valor = ahorro;
                ahorros.push_back(a);
            }
        }
    }

    // Si no hay ahorros positivos, devolver rutas individuales
    if (ahorros.empty()) {
        vector<vector<int>> resultado_sin_fusion;
        for (int k = 1; k <= dimension; ++k) {
            if (rutas[k].size() > 2) {
                resultado_sin_fusion.push_back(rutas[k]);
            }
        }
        
        Solution solution(instancia);
        solution._rutas = resultado_sin_fusion;
        
        // Calcular demandas y distancias
        for (int i = 0; i < resultado_sin_fusion.size(); ++i) {
            const vector<int>& ruta = resultado_sin_fusion[i];
            
            int demanda_total = 0;
            for (int j = 1; j < ruta.size() - 1; ++j) {
                demanda_total += demandas[ruta[j]];
            }
            solution._sumd.push_back(demanda_total);
            
            double distancia_total = 0.0;
            for (int j = 0; j < ruta.size() - 1; ++j) {
                distancia_total += dist[ruta[j]][ruta[j + 1]];
            }
            solution._distancias.push_back(distancia_total);
        }
        
        return solution;
    }

    // Ordenar ahorros de mayor a menor usando std::sort
    sort(ahorros.begin(), ahorros.end(), 
         [](const Ahorro& a, const Ahorro& b) { 
             return a.valor > b.valor; 
         });

    // Procesar ahorros para fusionar rutas
    for (const Ahorro& ahorro : ahorros) {
        int i = ahorro.i;
        int j = ahorro.j;

        // Encontrar posiciones de i y j en las rutas
        auto pos_i = encontrar_nodo_en_rutas(rutas, i, dimension);
        auto pos_j = encontrar_nodo_en_rutas(rutas, j, dimension);
        
        int ruta_i = pos_i.first;
        int ruta_j = pos_j.first;
        int posicion_i = pos_i.second;
        int posicion_j = pos_j.second;

        // Verificar si pueden fusionarse
        if (!pueden_fusionarse(rutas, i, j, ruta_i, posicion_i, ruta_j, posicion_j)) {
            continue;
        }

        // Verificar restricción de capacidad
        if (demanda_ruta[ruta_i] + demanda_ruta[ruta_j] > capacidad) {
            continue;
        }

        // Determinar el orden de fusión
        vector<int> nueva_ruta;
        nueva_ruta.push_back(deposito);

        // Caso 1: i al final de ruta_i, j al inicio de ruta_j
        if (posicion_i == rutas[ruta_i].size() - 2 && posicion_j == 1) {
            // Agregar clientes de ruta_i (sin depósitos)
            for (int idx = 1; idx < rutas[ruta_i].size() - 1; ++idx) {
                nueva_ruta.push_back(rutas[ruta_i][idx]);
            }
            // Agregar clientes de ruta_j (sin depósitos)
            for (int idx = 1; idx < rutas[ruta_j].size() - 1; ++idx) {
                nueva_ruta.push_back(rutas[ruta_j][idx]);
            }
        }
        // Caso 2: j al final de ruta_j, i al inicio de ruta_i
        else if (posicion_j == rutas[ruta_j].size() - 2 && posicion_i == 1) {
            // Agregar clientes de ruta_j (sin depósitos)
            for (int idx = 1; idx < rutas[ruta_j].size() - 1; ++idx) {
                nueva_ruta.push_back(rutas[ruta_j][idx]);
            }
            // Agregar clientes de ruta_i (sin depósitos)
            for (int idx = 1; idx < rutas[ruta_i].size() - 1; ++idx) {
                nueva_ruta.push_back(rutas[ruta_i][idx]);
            }
        }
        else {
            continue; // No se puede fusionar en esta configuración
        }

        nueva_ruta.push_back(deposito);

        // Actualizar las estructuras de datos
        rutas[ruta_j] = nueva_ruta;
        demanda_ruta[ruta_j] = demanda_ruta[ruta_i] + demanda_ruta[ruta_j];
        
        // Limpiar la ruta fusionada
        rutas[ruta_i].clear();
        demanda_ruta[ruta_i] = 0;
    }

    // Recopilar rutas finales no vacías
    vector<vector<int>> resultado;
    for (int k = 1; k <= dimension; ++k) {
        if (!rutas[k].empty()) {
            resultado.push_back(rutas[k]);
        }
    }

    // Crear objeto Solution
    Solution solution(instancia);
    solution._rutas = resultado;

    // Calcular demandas y distancias para cada ruta
    for (int i = 0; i < resultado.size(); ++i) {
        const vector<int>& ruta = resultado[i];
        
        // Calcular demanda total
        int demanda_total = 0;
        for (int j = 1; j < ruta.size() - 1; ++j) { // Excluir depósitos
            demanda_total += demandas[ruta[j]];
        }
        solution._sumd.push_back(demanda_total);
        
        // Calcular distancia total
        double distancia_total = 0.0;
        for (int j = 0; j < ruta.size() - 1; ++j) {
            distancia_total += dist[ruta[j]][ruta[j + 1]];
        }
        solution._distancias.push_back(distancia_total);
    }

    return solution;
}