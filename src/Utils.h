#ifndef UTILS_H
#define UTILS_H
#include "VRPLIBReader.h"
#include "Solution.h"
#include <vector>
#include <algorithm>
using namespace std;

double calcularCostoTotal(Solution sol);

void removeNodeFromVector(vector<int>& v, int n);

void printRuta (vector<int>& v);

#endif