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
        cout<<closest_node<<"\n";

        double best_cost = 100000000;
        int best_ruta = -1;

        vector<vector<int>> rutas = sol.getRutas();

        Solution sol2 = sol;

        double costoConRutaNueva = calcularCostoTotal(sol2);
        sol2.addRuta(closest_node);
        
        for (int i = 0; i < rutas.size(); i++) {
            int original_size = rutas[i].size();
            
            // Add at second to last position
            sol.addClient(closest_node, i, original_size - 1);
            sol.printSolution();
            // Remove the same element we just added (at same logical position)
            sol.removeClient(i, original_size - 1); // Same position after addition
            sol.printSolution();
        
        }

        sol = sol2;
        removeNodeFromVector(unvisited, closest_node);
    }
    cout<<"done\n";
    sol.printSolution();
    return sol;

}

