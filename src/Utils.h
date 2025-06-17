#ifndef UTILS_H
#define UTILS_H
#include "VRPLIBReader.h"

#include <vector>

using namespace std;

double calcularCostoTotal(const vector<vector<int>>& rutas, const vector<vector<double>>& matriz_distancias);

#endif