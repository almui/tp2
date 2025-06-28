#ifndef GRASP_H
#define GRASP_H

#include <vector>
#include "VRPLIBReader.h"
#include "Relocate.h"
#include "Solution.h"
#include "Utils.h"
#include <iostream>
#include <cstdlib> // Para rand() 
#include <ctime>   // Para time()

using namespace std;

// Realiza el algoritmo GRASP repitiendo n veces un algoritmo goloso aleatorio y una mejora local de tipo "relocate".
// Devuelve la mejor solución encontrada en todas las iteraciones.
Solution grasp(int n, int rcl_size, const VRPLIBReader& instancia);
Solution RandomizedNearestNeighbor(int rcl, const VRPLIBReader& instancia); // Crea una solución inicial con un algoritmo greedy aleatorizado usando vecinos más cercanos desde la RCL.
vector<int> candidatos_validos(int id, int k, int capacidad, vector<int> v,const VRPLIBReader& instancia ); // Devuelve hasta k clientes no visitados más cercanos al nodo id que no superen la capacidad.


#endif // GRASP_H
