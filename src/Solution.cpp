#include "Solution.h"

Solution::Solution(const string& filePath) {
    _instancia= VRPLIBReader(filePath);
    _sol=vector<int>(_instancia.getDimension());
    _rutas={};
    _sumd={};
    _distancias={};
}

void Solution::addRuta(int id){
    _rutas.push_back({id});
    _sol[0].push_back(id);
    _sol[id].push_back(0);
    _sumd.push_back(_instancia.getDemands()[id]);
    _distancias.push_back(_instancia.getDistanceMatrix()[0][id] + _instancia.getDistanceMatrix()[id][0]);
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
    if(contain(adelante, _sol[atras])){
        _sol[atras].pop_back(); //solo tendrian que estar apuntando a un nodo, osea solo tendria un vector de un elemento 
    }
    _sol[atras].push_back(id);
    _sol[id].push_back(adelante);
    int pos= posicion(adelante, _rutas[ruta]);
    _rutas[ruta].insert(_rutas[ruta].begin() + pos, id);
    _sumd[ruta]+=_instancia.getDemands()[id];
    _distancias[ruta]+=_instancia.getDistanceMatrix()[atras][id] + _instancia.getDistanceMatrix()[id][adelante];
    
}
void Solution::removeClient(int id, int ruta,int atras, int adelante){
    _sol[atras].pop_back();
    _sol[id].pop_back();
    _sol[atras].push_back(adelante);
    int pos= posicion(id, _rutas[ruta]);
    _rutas[ruta].erase(_rutas[ruta].begin() + pos);
    _sumd[ruta]-=_instancia.getDemands()[id];
    _distancias[ruta]-=_instancia.getDistanceMatrix()[atras][id] + _instancia.getDistanceMatrix()[id][adelante];
}
bool Solution::esValida(int ruta){
    if(_sumd[ruta]<=_intancia.getCapacity()){
        return true;
    } else{
        return false;

    }
}
vector<int> Solution::getRutas(){
    return _rutas
}
vector<int> Solution::getDistancias(){
    return _distancias;
}
vector<int> Solution::getInstancia(){
    return _instancia;
}