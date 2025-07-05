#include <iostream>
#include <string>
#include <algorithm>
#include "VRPLIBReader.h"
#include "ClarkeWright.h"
#include "InsertionHeuristic.h"
#include "Solution.h"
#include "Relocate.h"
#include "Swap.h"

int main(int argc, char* argv[]) {
    srand(time(0));

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <path_to_vrp_file> <algorithm: clarke_wright|insertion|meta_insertion> [swap: true|false] [relocate: true|false]\n";
        return 1;
    }

    std::string path = argv[1];
    std::string algorithm = argv[2];
    std::transform(algorithm.begin(), algorithm.end(), algorithm.begin(), ::tolower); // normalize

    bool use_swap = (argc >= 4 && std::string(argv[3]) == "true");
    bool use_relocate = (argc >= 5 && std::string(argv[4]) == "true");

    VRPLIBReader reader(path);

    std::cout << "Instance Name: " << reader.getName() << std::endl;
    std::cout << "Dimension: " << reader.getDimension() << std::endl;
    std::cout << "Number of Vehicles: " << reader.getNumVehicles() << std::endl;
    std::cout << "Capacity: " << reader.getCapacity() << std::endl;
    std::cout << "Depot ID: " << reader.getDepotId() << std::endl;

    std::cout << "Distance matrix:" << std::endl;
    auto dist_matrix = reader.getDistanceMatrix();
    for (int i = 1; i <= reader.getDimension(); ++i) {
        for (int j = 1; j <= reader.getDimension(); ++j) {
            std::cout << dist_matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    Solution sol(reader); // default, in case nothing runs

    if (algorithm == "clarke_wright") {
        std::cout << "\nRunning Clarke-Wright Heuristic...\n";
        sol = clarke_wright(reader);
    } else if (algorithm == "insertion") {
        std::cout << "\nRunning Insertion Heuristic...\n";
        sol = insertion(reader);
    } else if (algorithm == "meta_insertion") {
        std::cout << "\nRunning Meta Insertion Heuristic...\n";
        sol = meta_insertion(reader);
    } else {
        std::cerr << "Unknown algorithm: " << algorithm << "\n";
        return 1;
    }

    std::cout << "\nInitial solution:\n";
    sol.printSolution();

    if (use_relocate) {
        std::cout << "\nApplying Relocate...\n";
        sol = relocate(sol);
        sol.printSolution();
    }

    if (use_swap) {
        std::cout << "\nApplying Swap...\n";
        sol = route_swap(sol);
        sol.printSolution();
    }

    return 0;
}
