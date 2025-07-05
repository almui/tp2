#ifndef INSERTIONHEURISTIC_H
#define INSERTIONHEURISTIC_H
#include "VRPLIBReader.h"
#include "Solution.h"
#include "Utils.h"
#include <vector>

using namespace std;
//devuelve el nodo no visitado mas cercano al deposito
int findClosestUnvisited(const VRPLIBReader& instancia, vector<int> unvisited);
int random3Closest (const VRPLIBReader& instancia, vector<int> unvisited);
Solution insertion(const VRPLIBReader& instancia);
Solution meta_insertion(const VRPLIBReader& instancia);

#endif