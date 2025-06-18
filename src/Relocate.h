#ifndef RELOCATE_H
#define RELOCATE_H

#include <vector>
#include "VRPLIBReader.h"

using namespace std;

// Realiza una mejora local tipo "relocate" en las rutas, manteniendo las restricciones de capacidad.
// Las rutas deben estar en el formato: rutas[i] = {0, cliente1, ..., clienteN, 0}
void relocate(vector<vector<int>>& rutas, const VRPLIBReader& instancia);

#endif // RELOCATE_H
