#ifndef INSERTIONHEURISTIC_H
#define INSERTIONHEURISTIC_H
#include "VRPLIBReader.h"
#include "Solution.h"
#include "Utils.h"
#include <vector>

using namespace std;

double findClosestUnvisited(const VRPLIBReader& instancia, vector<int> unvisited);

Solution insertion(const VRPLIBReader& instancia);

#endif