#include "Solution.h"

Solution::Solution(const VRPLIBReader& instancia) : _instancia(instancia) {
    _rutas = {};
    _sumd = {};
    _distancias = {};
}

void Solution::addRuta(int id) {
    _rutas.push_back({1, id, 1});
    vector<int> demandas = _instancia.getDemands();
    _sumd.push_back(demandas[id-1]);
    _distancias.push_back(_instancia.getDistanceMatrix()[1][id-1] + _instancia.getDistanceMatrix()[id-1][1]);
}

bool Solution::contain(int id, vector<int> ruta) {
    for (int i = 0; i < ruta.size(); i++) {
        if (ruta[i] == id) {
            return true;
        }
    }
    return false;
}

int Solution::posicion(int id, vector<int> v) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == id) {
            return i;
        }
    }
    return -1;
}

void Solution::addClient(int id, int ruta, int posicion) {
    vector<int>& r = _rutas[ruta];
    
    // Insert client at the specified position in the route
    r.insert(r.begin() + posicion, id);
    
    // Update demand
    _sumd[ruta] += _instancia.getDemands()[id];
    
    // Update distance
    const auto& dist = _instancia.getDistanceMatrix();
    int anterior = r[posicion - 1];
    int siguiente = r[posicion + 1];
    
    // Remove old distance (anterior -> siguiente) and add new distances (anterior -> id -> siguiente)
    _distancias[ruta] -= dist[anterior][siguiente];
    _distancias[ruta] += dist[anterior][id] + dist[id][siguiente];
}

void Solution::removeClient(int ruta, int posicion) {
    vector<int>& r = _rutas[ruta];
    int id = r[posicion];
    
    // Update demand
    _sumd[ruta] -= _instancia.getDemands()[id];
    
    // Update distance
    const auto& dist = _instancia.getDistanceMatrix();
    int anterior = r[posicion - 1];
    int siguiente = r[posicion + 1];
    
    _distancias[ruta] -= dist[anterior][id] + dist[id][siguiente];
    _distancias[ruta] += dist[anterior][siguiente];
    
    // Remove client from route
    r.erase(r.begin() + posicion);
}


bool Solution::esValida(int ruta) {
    return (_sumd[ruta] <= _instancia.getCapacity() && _rutas.size() <= _instancia.getNumVehicles());
}

vector<int> Solution::getDemandas() const {
    return _sumd;
}

vector<vector<int>> Solution::getRutas() const {
    return _rutas;
}

vector<double> Solution::getDistancias() const {
    return _distancias;
}

VRPLIBReader Solution::getInstancia() const {
    return _instancia;
}

void Solution::printSolution() const {
    string nombre = _instancia.getName();
    int depot = _instancia.getDepotId();
    double totalCost = 0.0;
    
    for (int ruta = 0; ruta < _distancias.size(); ruta++) {
        totalCost += _distancias[ruta];
    }
    
    cout << "NAME : " << nombre << endl;
    cout << "ROUTES : " << _rutas.size() << endl;
    cout << "COST : " << totalCost << endl;
    cout << "SOLUTION_SECTION" << endl;
    cout << " #R SUMD COST LENGTH #C SEQUENCE" << endl;
    
    for (int i = 0; i < _rutas.size(); ++i) {
        cout << setw(3) << i + 1 << " "
             << setw(5) << _sumd[i] << " "
             << setw(10) << _distancias[i] << " "
             << setw(10) << _distancias[i] << " "
             << setw(4) << _rutas[i].size() << " ";
             
        for (int j = 0; j < _rutas[i].size(); ++j) {
            cout << _rutas[i][j] << " ";
        }
        cout << endl;
    }
    
    cout << "DEPOT_SECTION" << endl;
    cout << depot << endl;
    cout << "END" << endl;
}