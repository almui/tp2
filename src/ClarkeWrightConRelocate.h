#ifndef CLARKEWRIGHTCONRELOCATE_H
#define CLARKEWRIGHTCONRELOCATE_H
#include "VRPLIBReader.h"

#include <vector>
using namespace std;
// Requiere que tengas esta clase disponible:

struct Ahorro {
    int i;
    int j;
    double valor;
};

// Ordena los ahorros de mayor a menor
void ordenar_ahorros(vector<Ahorro>& ahorros);

// Algoritmo  de Clarke & Wright con relocate
vector<vector<int>> clarke_wright(const VRPLIBReader& instancia);

double calcularCostoTotal(const vector<vector<int>>& rutas, const vector<vector<double>>& matriz_distancias);

// Mejora la solución reubicando nodos entre rutas
void relocate(std::vector<std::vector<int>>&, const VRPLIBReader&);


#endif 
