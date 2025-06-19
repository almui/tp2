#include "Solution.h"

Solution::Solution(const VRPLIBReader& instancia) : _instancia(instancia)  {
    _sol = vector<vector<int>>(instancia.getDimension());
    _rutas={};
    _sumd={};
    _distancias={};
}

void Solution::addRuta(int id){
    _rutas.push_back({1, id, 1});
    _sol[0].push_back(id);
    _sol[id].push_back(1);
    vector<int> demandas=_instancia.getDemands();
    _sumd.push_back(demandas[id+1]);
    _distancias.push_back(_instancia.getDistanceMatrix()[1][id+1] + _instancia.getDistanceMatrix()[id+1][1]);
}
bool Solution::contain(int id, vector<int> ruta){
    for(int i=0;i<ruta.size(); i++){
        if(ruta[i]==id){
            return true;
        }
    }
    return false; 

}
int Solution::posicion(int id, vector<int> v){
    for(int i=0; i<v.size(); i++){
        if(v[i]==id){
            return i;
        }
    }
}
void Solution::addClient(int id, int ruta, int atras, int adelante){
    
    if(contain(adelante, _sol[atras]) && atras!=1){
        int pos= posicion(adelante, _sol[atras]);
        _sol[atras].erase(_sol[atras].begin() + pos);

    }

    _sol[atras].push_back(id);
    _sol[id].push_back(adelante);
    _sumd[ruta]+=_instancia.getDemands()[id+1];

 
    if (adelante == 1) {
        _rutas[ruta].insert(_rutas[ruta].end() - 1, id);
        _distancias[ruta]-=_instancia.getDistanceMatrix()[atras+1][1];
        _distancias[ruta]+=_instancia.getDistanceMatrix()[atras+1][id+1] + _instancia.getDistanceMatrix()[id+1][1];
    } else if (atras == 1) {
        _rutas[ruta].insert(_rutas[ruta].begin() + 1 , id);
        _distancias[ruta]-=_instancia.getDistanceMatrix()[1][adelante+1];
        _distancias[ruta]+=_instancia.getDistanceMatrix()[1][id+1] + _instancia.getDistanceMatrix()[id+1][adelante+1];

    } else {
        int pos = posicion(adelante, _rutas[ruta]);
        _rutas[ruta].insert(_rutas[ruta].begin() + pos, id);
        _distancias[ruta]-=_instancia.getDistanceMatrix()[atras+1][adelante+1];
        _distancias[ruta]+=_instancia.getDistanceMatrix()[atras+1][id+1] + _instancia.getDistanceMatrix()[id+1][adelante+1];
    }
}
void Solution::removeClient(int id, int ruta,int atras, int adelante){
    _sol[atras].pop_back();
    _sol[id].pop_back();
    _sol[atras].push_back(adelante);
    int pos= posicion(id, _rutas[ruta]);
    _rutas[ruta].erase(_rutas[ruta].begin() + pos);
    _sumd[ruta]-=_instancia.getDemands()[id+1];
    _distancias[ruta]-=_instancia.getDistanceMatrix()[atras+1][id+1] + _instancia.getDistanceMatrix()[id+1][adelante+1];
    _distancias[ruta]+=_instancia.getDistanceMatrix()[atras+1][adelante+1];
}
bool Solution::esValida(int ruta){
    if(_sumd[ruta]<=_instancia.getCapacity()){
        return true;
    } else{
        return false;

    }
}
vector<int> Solution::getDemandas(){
    return _sumd;
}
vector<vector<int>> Solution::getRutas(){
    return _rutas;
}
vector<double> Solution::getDistancias(){
    return _distancias;
}
VRPLIBReader Solution::getInstancia(){
    return _instancia;
}
void Solution::printSolution() const{
    string nombre = _instancia.getName();
    int depot = _instancia.getDepotId();

    double totalCost = 0.0;
    for (int ruta=0; ruta < _distancias.size(); ruta++) {
        totalCost += _distancias[ruta];
    }
    cout << "NAME    : " << nombre << endl;
    cout << "ROUTES  : " << _rutas.size() << endl;
    cout << "COST    : " << totalCost << endl;
    cout << "SOLUTION_SECTION" << endl;
    cout << " #R   SUMD        COST     LENGTH   #C     SEQUENCE" << endl;

    for (int i = 0; i < _rutas.size(); ++i) {

        cout << setw(3) << i + 1 << "  "
             << setw(5) << _sumd[i] << "  "
             << setw(10) << _distancias[i] << "  "
             << setw(10) << _distancias[i] << "  "
             << setw(4) << _rutas[i].size() << "     ";

        for (int j = 1; j <_rutas[i].size()-1; ++j) {
            cout << _rutas[i][j] << " ";
        }
        cout << endl;
    }

    cout << "DEPOT_SECTION" << endl;
    cout << depot << endl;
    cout << "END" << endl;
}

