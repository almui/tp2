#ifndef UTILS_H
#define UTILS_H
#include "VRPLIBReader.h"
#include "Solution.h"
#include <vector>

using namespace std;

double calcularCostoTotal(Solution sol);

int calcularDemandaRuta(Solution sol, int indexRuta);

#endif