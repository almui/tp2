#ifndef RELOCATE_H
#define RELOCATE_H

#include <vector>
#include "VRPLIBReader.h"
#include "Solution.h"

using namespace std;

// Realiza una mejora local tipo "relocate" en las rutas, manteniendo las restricciones de capacidad.
// Las rutas deben estar en el formato: rutas[i] = {0, cliente1, ..., clienteN, 0}
Solution relocate(const Solution initial_sol);

#endif // RELOCATE_H
