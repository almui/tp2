#include "InsertionHeuristic.h"
#include "VRPLIBReader.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int findClosestUnvisited(const VRPLIBReader& instancia, vector<int> unvisited){
    //unvisited es la lista de nodos que todav no estan en ninguna ruta
    //inicia con todos en ella salvo el 0 (el deposito)
    vector<vector<int>> distancias = instancia.distanceMatrix();
    int min_d = 1000000000;
    for (int i=0; i<unvisited.size(); i++){
        if (distancias[0][unvisited[i]]<min_d ){
            min_d = distancias[0][unvisited[i]];
        }
    }
    return min_d;
}

Solution insertion(const VRPLIBReader& instancia){

}

