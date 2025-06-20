#ifndef CLARKEWRIGHT_H
#define CLARKEWRIGHT_H
#include "VRPLIBReader.h"
#include "Solution.h"

#include <vector>
using namespace std;

struct Ahorro {
    int i;
    int j;
    double valor;
};

    void ordenar_ahorros(vector<Ahorro>& ahorros);

// Algoritmo  de Clarke & Wright
    Solution clarke_wright(const VRPLIBReader& instancia);

// Ordena los ahorros de mayor a men

#endif 
