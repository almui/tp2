#include "ClarkeWright.h"
#include "Solution.h"
#include "VRPLIBReader.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// Sort savings from highest to lowest
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

// Initialize individual routes for each customer
void inicializar_rutas_individuales(vector<vector<int>>& rutas, vector<int>& demanda_ruta, const VRPLIBReader& instancia) {
    int deposito = instancia.getDepotId();
    int dimension = instancia.getDimension();
    const vector<int>& demandas = instancia.getDemands();
    
    // Add bounds checking
    if (demandas.size() <= dimension) {
        cerr << "Error: Demands vector size is insufficient" << endl;
        return;
    }
    
    for (int i = 1; i < dimension; ++i) {
        if (i == deposito) continue;
        rutas[i] = {deposito, i, deposito};
        // Fixed: Use proper indexing for demands array
        demanda_ruta[i] = demandas[i];  // Changed from demandas[i] to match your Solution.cpp indexing
    }
}

// Calculate and generate all possible savings
vector<Ahorro> calcular_ahorros(const VRPLIBReader& instancia) {
    vector<Ahorro> ahorros;
    int deposito = instancia.getDepotId();
    int dimension = instancia.getDimension();
    const auto& dist = instancia.getDistanceMatrix();
    
    // Add bounds checking for distance matrix
    if (dist.size() <= dimension) {
        cerr << "Error: Distance matrix size is insufficient" << endl;
        return ahorros;
    }
    
    for (int i = 1; i < dimension; ++i) {
        if (i == deposito) continue;
        
        // Check if row exists
        if (dist[i].size() <= dimension) {
            cerr << "Error: Distance matrix row " << i << " is insufficient" << endl;
            continue;
        }
        
        for (int j = i + 1; j < dimension; ++j) {
            if (j == deposito) continue;
            
            // Additional bounds checking
            if (dist[deposito].size() <= max(i, j)) {
                cerr << "Error: Distance matrix depot row is insufficient" << endl;
                continue;
            }
            
            double ahorro = dist[deposito][i] + dist[deposito][j] - dist[i][j];
            ahorros.push_back({i, j, ahorro});
        }
    }
    
    return ahorros;
}

// Convert internal routes to Solution format
Solution convertir_rutas_a_solucion(const vector<vector<int>>& rutas, const VRPLIBReader& instancia) {
    Solution sol(instancia);
    int dimension = instancia.getDimension();
    
    for (int k = 1; k < dimension; k++) {
        if (!rutas[k].empty() && rutas[k].size() >= 3) {
            const vector<int>& ruta_vec = rutas[k];
            
            // Add bounds checking
            if (ruta_vec.size() < 3) continue;
            
            int cliente_inicial = ruta_vec[1];
            sol.addRuta(cliente_inicial);

            // Add remaining clients to the route
            for (int i = 2; i < ruta_vec.size() - 1; i++) {
                int actual = ruta_vec[i];
                
                // Check if we have a next element
                if (i + 1 >= ruta_vec.size()) break;
                int adelante = ruta_vec[i + 1];

                // Get current routes and check bounds
                auto current_routes = sol.getRutas();
                if (current_routes.empty()) break;
                
                int ruta_idx = current_routes.size() - 1;
                const vector<int>& r = current_routes[ruta_idx];
                
                // Find position more safely
                auto pos_it = find(r.begin(), r.end(), adelante);
                if (pos_it == r.end()) {
                    // If adelante is not found, add at the end before depot
                    int posicion = r.size() - 1;
                    sol.addClient(actual, ruta_idx, posicion);
                } else {
                    int posicion = pos_it - r.begin();
                    sol.addClient(actual, ruta_idx, posicion);
                }
            }
        }
    }
    
    return sol;
}

// Handle case when no positive savings exist
Solution crear_solucion_sin_fusion(const vector<vector<int>>& rutas, const VRPLIBReader& instancia) {
    return convertir_rutas_a_solucion(rutas, instancia);
}

// Find which route contains customer i at the beginning and customer j at the end
pair<int, int> encontrar_rutas_fusionables(int i, int j, const vector<vector<int>>& rutas, int dimension) {
    int ruta_i = -1, ruta_j = -1;

    for (int k = 1; k < dimension; ++k) {
        // Add bounds checking
        if (k >= rutas.size()) continue;
        if (rutas[k].empty() || rutas[k].size() < 3) continue;
        
        // Check if route starts with customer i (position 1, after depot)
        if (rutas[k][1] == i) ruta_i = k;
        
        // Check if route ends with customer j (second to last position, before depot)
        if (rutas[k][rutas[k].size() - 2] == j) ruta_j = k;
    }

    return make_pair(ruta_i, ruta_j);
}

// Check if two routes can be merged based on capacity constraint
bool puede_fusionar_rutas(int ruta_i, int ruta_j, const vector<int>& demanda_ruta, int capacidad) {
    // Add bounds checking
    if (ruta_i < 0 || ruta_j < 0 || ruta_i >= demanda_ruta.size() || ruta_j >= demanda_ruta.size()) {
        return false;
    }
    
    return ruta_i != ruta_j && demanda_ruta[ruta_i] + demanda_ruta[ruta_j] <= capacidad;
}

// Merge two routes by combining route_i into route_j
void fusionar_rutas(int ruta_i, int ruta_j, vector<vector<int>>& rutas, vector<int>& demanda_ruta, int deposito) {
    // Add bounds checking
    if (ruta_i < 0 || ruta_j < 0 || ruta_i >= rutas.size() || ruta_j >= rutas.size()) {
        return;
    }
    if (rutas[ruta_i].empty() || rutas[ruta_j].empty()) {
        return;
    }
    
    vector<int>& r_i = rutas[ruta_i];
    vector<int>& r_j = rutas[ruta_j];

    // Check minimum sizes
    if (r_i.size() < 3 || r_j.size() < 3) {
        return;
    }

    // Remove depot from end of route j
    r_j.pop_back();
    
    // Add all customers from route i to route j (excluding depots)
    for (int idx = 1; idx < r_i.size() - 1; ++idx) {
        r_j.push_back(r_i[idx]);
    }
    
    // Add depot at the end
    r_j.push_back(deposito);

    // Update demand and clear route i
    if (ruta_i < demanda_ruta.size() && ruta_j < demanda_ruta.size()) {
        demanda_ruta[ruta_j] += demanda_ruta[ruta_i];
        demanda_ruta[ruta_i] = 0;
    }
    rutas[ruta_i].clear();
}

// Apply savings algorithm to merge routes
void aplicar_algoritmo_ahorros(vector<Ahorro>& ahorros, vector<vector<int>>& rutas, vector<int>& demanda_ruta, const VRPLIBReader& instancia) {
    int dimension = instancia.getDimension();
    int capacidad = instancia.getCapacity();
    int deposito = instancia.getDepotId();

    for (const Ahorro& a : ahorros) {
        int i = a.i, j = a.j;
        
        pair<int, int> rutas_encontradas = encontrar_rutas_fusionables(i, j, rutas, dimension);
        int ruta_i = rutas_encontradas.first;
        int ruta_j = rutas_encontradas.second;

        if (puede_fusionar_rutas(ruta_i, ruta_j, demanda_ruta, capacidad)) {
            fusionar_rutas(ruta_i, ruta_j, rutas, demanda_ruta, deposito);
        }
    }
}

// Main Clarke & Wright algorithm implementation
Solution clarke_wright(const VRPLIBReader& instancia) {
    int deposito = instancia.getDepotId();
    int dimension = instancia.getDimension();
    int capacidad = instancia.getCapacity();

    // Add basic validation
    if (dimension <= 1) {
        cerr << "Error: Invalid dimension" << endl;
        return Solution(instancia);
    }

    // Initialize data structures with proper size
    vector<vector<int>> rutas(dimension + 1);
    vector<int> demanda_ruta(dimension + 1, 0);

    // Step 1: Create individual routes for each customer
    inicializar_rutas_individuales(rutas, demanda_ruta, instancia);

    // Step 2: Calculate all possible savings
    vector<Ahorro> ahorros = calcular_ahorros(instancia);

    // Check if we have any savings
    if (ahorros.empty()) {
        return crear_solucion_sin_fusion(rutas, instancia);
    }

    // Step 3: Sort savings in descending order
    ordenar_ahorros(ahorros);

    // Step 4: Handle case with no positive savings
    if (ahorros[0].valor <= 0) {
        return crear_solucion_sin_fusion(rutas, instancia);
    }

    // Step 5: Apply savings algorithm to merge routes
    aplicar_algoritmo_ahorros(ahorros, rutas, demanda_ruta, instancia);

    // Step 6: Convert final routes to Solution format
    return convertir_rutas_a_solucion(rutas, instancia);
}