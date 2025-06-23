#include <iostream>
#include "VRPLIBReader.h"
#include "ClarkeWright.h"
#include "Solution.h"
#include "InsertionHeuristic.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_vrp_file>" << std::endl;
        return 1;
    }

    VRPLIBReader reader(argv[1]);

    std::cout << "Instance Name: " << reader.getName() << std::endl;
    std::cout << "Dimension: " << reader.getDimension() << std::endl;
    std::cout << "Number of Vehicles: " << reader.getNumVehicles() << std::endl;
    std::cout << "Capacity: " << reader.getCapacity() << std::endl;
    // Notar que el deposito varia y no siempre es 0. getDepotId() nos dice cual es el id del deposito.
    std::cout << "Depot ID: " << reader.getDepotId() << std::endl;

    std::vector<Node> clients = reader.getNodes();

    std::cout << clients.size() << std::endl;
    
    // Testeamos la matriz de distancia. Tomando los id de los nodos, indexa desde 1.
    // Tener en cuenta esto al momento de decidir como representar una solucion.
    std::cout << "Distance matrix" << std::endl;
    std::vector<std::vector<double>> dist_matrix = reader.getDistanceMatrix();
    for (int i = 1; i <= reader.getDimension(); i++) {
        for (int j = 1; j <= reader.getDimension(); j++) {
            std::cout << dist_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "\nEjecutando Heuristica de insercion...\n" << std::endl;
    Solution sol = clarke_wright(reader);
    std::vector<vector<int>> rutas = sol.getRutas();
    // vector<vector<int>> rutas = clarke_wright(reader);
    // std::cout << "Rutas generadas:\n";
    // for (size_t i = 0; i < rutas.size(); ++i) {
    //     std::cout << "Ruta " << i + 1 << ": ";
    //     for (int cliente : rutas[i]) {
    //         std::cout << cliente << " ";
    //     }
    //     std::cout << std::endl;
    // }
    sol.printSolution();
    return 0;
}

