#include "Grasp.h"


Solution grasp(int n, int rcl_size, const VRPLIBReader& instancia){
    Solution best(instancia);
    double costo_min=99999999999.9;
    for(int i=0; i<n; i++){
        Solution current_sol = RandomizedNearestNeighbor(rcl_size, instancia);// genero solución random greedy
        current_sol= relocate(current_sol); // aplico relocate y si es mejor lo guardo 
        if(costo_min > calcularCostoTotal(current_sol)){
            best= current_sol;
            costo_min= calcularCostoTotal(current_sol);
        }
    }
    return best;


}

Solution RandomizedNearestNeighbor(int rcl, const VRPLIBReader& instancia) {
    vector<vector<double>> matriz = instancia.getDistanceMatrix();
    int id= instancia.getDepotId();
    vector<int> visitados = { id};
    vector<int> no_visitados = {};

    for (int i = 2; i <= instancia.getDimension(); i++) {// meto todos los clientes (sin el deposito)
        no_visitados.push_back(i);
    }
    srand(time(0));  // semilla para la funcion random

    int num_ruta = -1;
    Solution sol(instancia);

    while (!no_visitados.empty()) { // mientras queden clientes sin vistar
        int id= instancia.getDepotId();
        vector<int> candidatos = candidatos_validos(id, rcl,0, no_visitados, instancia);// armo los RCL candidatos validos, los clientes más cercanos desde el deposito
        if (candidatos.empty()) {

            continue; // si no hay candidatos, sigo
        }

        int randomNumber = rand() % candidatos.size(); // elijo un numero random del 0 al tamaño de candidatos -1
        id = candidatos[randomNumber];
        sol.addRuta(id);           // empiezo nueva ruta
        visitados.push_back(id);
        removeNodeFromVector(no_visitados, id);



        bool ruta_abierta = true;
        int pos_cliente = 2;          // posición dentro de la ruta, despues de deposito y el cliente con el que iniciamos la ruta
        num_ruta++; // para saber en que ruta estamos
        int capacidad=instancia.getDemands()[id];
        while (ruta_abierta) {
            id = visitados.back();    // último cliente agregado
            
            candidatos = candidatos_validos(id,rcl, capacidad, no_visitados, instancia);

            if (candidatos.empty()) { //si esta vacio cierro la ruta, porque no tenemos mas candidatos
                ruta_abierta = false;
            } else {
                int randomNumber = rand() % candidatos.size();
                id = candidatos[randomNumber];

                sol.addClient(id, num_ruta, pos_cliente);
                visitados.push_back(id);
                removeNodeFromVector(no_visitados, id);
                capacidad+= instancia.getDemands()[id]; // sumo la demanda para podes saber si la ruta es valida


                pos_cliente++;
            }
        }
    }

    return sol;
}
vector<int> candidatos_validos(int id, int k, int capacidad, vector<int> v,const VRPLIBReader& instancia ){
    vector<double> distancias_id = instancia.getDistanceMatrix()[id];
    vector<int> demandas=instancia.getDemands();
    vector<int> res={};
    vector<int> restantes= v;

    while (res.size() <=k && !restantes.empty()){ // mientras no lleguemos al tamaño k y queden nodos para agregar 
        int candidato= -1;
        double min_d = 99999999999.9;
        for (int i=0; i<restantes.size(); i++){
            // si el nodo está más cerca y no supera la capacidad lo guardo 
            if (distancias_id[restantes[i]] < min_d && capacidad + demandas[restantes[i]] <=instancia.getCapacity() ){ 
                min_d = distancias_id[restantes[i]];
                candidato = restantes[i];
            }
        }
        if (candidato != -1) {
            res.push_back(candidato);
            removeNodeFromVector(restantes, candidato);
        } else {
            break; // si no hay más candidatos válidos, salgo del for
        }
        
    }
    return res; 
}


