#include "Utils.h"
#include "Solution.h"

double calcularCostoTotal(Solution sol) {
    double costo_total = 0.0;
    vector<double> distancias = sol.getDistancias();

    for (double d : distancias) {
        costo_total += d;
    }

    return costo_total;
}
void removeNodeFromVector(vector<int>& v, int n) {
    auto it = std::find(v.begin(), v.end(), n);
    if (it != v.end()) {
        v.erase(it);
    }
}

void printRuta(vector<int>& v){
    for (int i=0; i<v.size(); i++){
        cout<<v[i]<<" ";
    }
    cout<<"\n";
}
