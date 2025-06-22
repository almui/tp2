#ifndef CLARKEWRIGHT_H
#define CLARKEWRIGHT_H
#include "VRPLIBReader.h"
#include "Solution.h"

#include <vector>
using namespace std;

struct Ahorro {
    int i;
    int j;
    double valor;
};

    void ordenar_ahorros(vector<Ahorro>& ahorros);

// Algoritmo  de Clarke & Wright
    Solution clarke_wright(const VRPLIBReader& instancia);

    void inicializar_rutas_individuales(vector<vector<int>>& rutas, vector<int>& demanda_ruta, const VRPLIBReader& instancia);

    vector<Ahorro> calcular_ahorros(const VRPLIBReader& instancia);

    Solution convertir_rutas_a_solucion(const vector<vector<int>>& rutas, const VRPLIBReader& instancia);

    Solution crear_solucion_sin_fusion(const vector<vector<int>>& rutas, const VRPLIBReader& instancia);

    pair<int, int> encontrar_rutas_fusionables(int i, int j, const vector<vector<int>>& rutas, int dimension);

    bool puede_fusionar_rutas(int ruta_i, int ruta_j, const vector<int>& demanda_ruta, int capacidad);

    void fusionar_rutas(int ruta_i, int ruta_j, vector<vector<int>>& rutas, vector<int>& demanda_ruta, int deposito);

    void aplicar_algoritmo_ahorros(vector<Ahorro>& ahorros, vector<vector<int>>& rutas, vector<int>& demanda_ruta, const VRPLIBReader& instancia);

    

// Ordena los ahorros de mayor a men

#endif 
