#ifndef SOLUTION_H
#define SOLUTION_H

#include "VRPLIBReader.h"
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class Solution {
public:
    // Make member variables public so clarke_wright can access them directly
    vector<vector<int>> _rutas;
    vector<int> _sumd;
    vector<double> _distancias;

private:
    VRPLIBReader _instancia;

public:
    Solution(const VRPLIBReader& instancia);
    
    void addRuta(int id);
    bool contain(int id, vector<int> ruta);
    int posicion(int id, vector<int> v);
    void addClient(int id, int ruta, int posicion);
    void removeClient(int ruta, int posicion);
    bool esValida(int ruta);
    
    vector<int> getDemandas() const;
    vector<vector<int>> getRutas() const;
    vector<double> getDistancias() const;
    VRPLIBReader getInstancia() const;
    
    void printSolution() const;
};

#endif