#ifndef SOLUTION
#define SOLUTION

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "VRPLIBReader.h"

using namespace std;

class Solution {
public:
    Solution(const VRPLIBReader& instancia);
    void addRuta(int id);
    void removeClient(int id, int ruta,int atras, int adelante);
    bool esValida(int ruta);
    vector<vector<int>> getRutas() const;
    vector<double> getDistancias() const;
    VRPLIBReader getInstancia() const;
    void printSolution() const;
    vector<int> getDemandas() const;
    void addClient(int id, int ruta, int posicion);
  
//id deposito = 1

private:
    vector<vector<int>> _sol; //grafo dirigido cada posicion es un nodo, y el vector en esa posicion son los nodos a los que apunta
    vector<vector<int>> _rutas;// las rutas ordenadas. Ejemplo: {2,8,4};{3,5,6};{1,7}
    vector<int> _sumd;
    vector<double> _distancias;
    VRPLIBReader _instancia;

    bool contain(int id, vector<int> ruta);
    int posicion(int id, vector<int> v);
};


#endif // SOLUTION 