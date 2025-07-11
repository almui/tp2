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

// Ordena los ahorros de mayor a menor
void ordenar_ahorros(vector<Ahorro>& ahorros);

// Algoritmo  de Clarke & Wright con relocate
Solution clarke_wright(const VRPLIBReader& instancia);


#endif 