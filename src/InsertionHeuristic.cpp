#include "InsertionHeuristic.h"
#include "Solution.h"
#include "VRPLIBReader.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int findClosestUnvisited(const VRPLIBReader& instancia, vector<int> unvisited){
    //unvisited es la lista de nodos que todav no estan en ninguna ruta
    //inicia con todos en ella salvo el 0 (el deposito)
    const auto& distancias = instancia.getDistanceMatrix();
    double min_d = 1000000000;
    int min_n = -1;
    for (int i=0; i<unvisited.size(); i++){
        if (distancias[1][unvisited[i]]<min_d ){
            min_d = distancias[1][unvisited[i]];
            min_n = unvisited[i];
        }
    }
    return min_n;
}

int random3Closest (const VRPLIBReader& instancia, vector<int> unvisited){
        const auto& distancias = instancia.getDistanceMatrix();
    
        if (unvisited.size() < 3) {
            return unvisited[0];
        }
    
        vector<pair<double, int>> distancias_nodos;
        for (int i = 0; i < unvisited.size(); i++) {
            distancias_nodos.push_back({distancias[1][unvisited[i]], unvisited[i]});
        }
    
        sort(distancias_nodos.begin(), distancias_nodos.end());
    
        int a = distancias_nodos[0].second;
        int b = distancias_nodos[1].second;
        int c = distancias_nodos[2].second;
    
        int r = rand() % 3;
        if (r == 0) return a;
        if (r == 1) return b;
        return c;
}
    


Solution insertion(const VRPLIBReader& instancia){
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

    //al principio ninguno de los nodos esta visitado
    vector<int> unvisited = {};
    for (int i=2; i<(dimension+1); i++){
        unvisited.push_back(i);
    }

    while (unvisited.size()>0){
        int closest_node = findClosestUnvisited(instancia, unvisited);
        double best_cost_insertando = 100000000;
        int best_ruta = -1;
        int best_position = -1;
        vector<vector<int>> rutas = sol.getRutas();
        Solution sol2 = sol;
        sol2.addRuta(closest_node);
        double costoConRutaNueva = calcularCostoTotal(sol2);
        
        for (int i = 0; i < rutas.size(); i++) {
            int original_size = rutas[i].size();
            for (int pos = 1; pos < original_size - 1; pos++) {
                sol.addClient(closest_node, i, pos);
                double tempCost = calcularCostoTotal(sol);
                if(tempCost < best_cost_insertando && sol.esValida(i)){
                    best_ruta = i;
                    best_position = pos;
                    best_cost_insertando = tempCost;
                }
                sol.removeClient(i, pos);
            }
        }
        
        if(best_cost_insertando < costoConRutaNueva){
            sol.addClient(closest_node, best_ruta, best_position);
        }
        else{
            sol = sol2;
        }
        removeNodeFromVector(unvisited, closest_node);
    }
    return sol;

}

Solution meta_insertion(const VRPLIBReader& instancia){
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

    //al principio ninguno de los nodos esta visitado
    vector<int> unvisited = {};
    for (int i=2; i<(dimension+1); i++){
        unvisited.push_back(i);
    }

    while (unvisited.size()>0){
        int closest_node = random3Closest(instancia, unvisited);
        double best_cost_insertando = 100000000;
        int best_ruta = -1;
        int best_position = -1;
        vector<vector<int>> rutas = sol.getRutas();
        Solution sol2 = sol;
        sol2.addRuta(closest_node);
        double costoConRutaNueva = calcularCostoTotal(sol2);
        
        for (int i = 0; i < rutas.size(); i++) {
            int original_size = rutas[i].size();
            for (int pos = 1; pos < original_size - 1; pos++) {
                sol.addClient(closest_node, i, pos);
                double tempCost = calcularCostoTotal(sol);
                if(tempCost < best_cost_insertando && sol.esValida(i)){
                    best_ruta = i;
                    best_position = pos;
                    best_cost_insertando = tempCost;
                }
                sol.removeClient(i, pos);
            }
        }
        
        if(best_cost_insertando < costoConRutaNueva){
            sol.addClient(closest_node, best_ruta, best_position);
        }
        else{
            sol = sol2;
        }
        removeNodeFromVector(unvisited, closest_node);
    }
    return sol;

}