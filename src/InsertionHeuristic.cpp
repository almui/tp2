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
        cout<<"a"<<closest_node<<"\n";

        double best_cost = 100000000;
        int best_ruta = 0;
        bool best_is_new = false;
        
        const auto original_rutas = sol.getRutas();
        const auto original_distancias = sol.getDistancias();
        const auto original_demandas = sol.getDemandas();

        //opcion insertar al final de cada ruta
        for (int i = 0; i < original_rutas.size(); i++) {
            const auto& ruta = original_rutas[i];

            Solution temp_sol = sol;
            
            temp_sol.addClient(closest_node, i, ruta.size() - 2); 
            double new_cost = calcularCostoTotal(temp_sol);
            if (new_cost < best_cost) {
                best_cost = new_cost;
                best_ruta = i;
                best_is_new = false;
            }
        }

        //opcion crear nueva ruta
        {
            Solution temp_sol = sol;
            temp_sol.addRuta(closest_node);
            double new_cost = calcularCostoTotal(temp_sol);
            if (new_cost < best_cost) {
                best_cost = new_cost;
                best_is_new = true;
            }
        }
        // Apply best option to actual solution
        if (best_is_new) {
            sol.addRuta(closest_node);
        } else {
            const auto& ruta = sol.getRutas()[best_ruta];
            sol.addClient(closest_node, best_ruta, ruta.size()-2);
        }

        // Remove node from unvisited
        removeNodeFromVector(unvisited, closest_node);
    }
    sol.printSolution();
    return sol;

}

