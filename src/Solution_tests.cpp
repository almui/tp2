#include <gtest/gtest.h>
#include "Solution.h"
#include "VRPLIBReader.h"

// Versión "fake" para testear sin leer archivos
class VRPLIBReaderFake : public VRPLIBReader {
public:
    VRPLIBReaderFake() {
        dimension = 3;
        capacity = 15;
        numVehicles = 2;
        depotId = 1;

        nodes = {
            {1, 0.0, 0.0}, // depot
            {2, 3.0, 4.0},
            {3, 6.0, 8.0}
        };

        demands = {0, 0, 5, 7}; // index 0 unused, node 1 depot demand=0

        // Precalcular matriz de distancias (4x4, index 0 no usado)
        distanceMatrix = std::vector<std::vector<double>>(4, std::vector<double>(4, 0));
        distanceMatrix[1][2] = 5.0; distanceMatrix[2][1] = 5.0;
        distanceMatrix[2][3] = 5.0; distanceMatrix[3][2] = 5.0;
        distanceMatrix[1][3] = 10.0; distanceMatrix[3][1] = 10.0;
    }

    // Sobrescribo los getters para que Solution pueda acceder a los datos
    const std::string& getName() const override {
        static std::string n = "FakeInstance";
        return n;
    }
    int getDimension() const override { return dimension; }
    int getCapacity() const override { return capacity; }
    int getNumVehicles() const override { return numVehicles; }
    int getDepotId() const override { return depotId; }
    const std::vector<Node>& getNodes() const override { return nodes; }
    const std::vector<int>& getDemands() const override { return demands; }
    const std::vector<std::vector<double>>& getDistanceMatrix() const override { return distanceMatrix; }

private:
    int dimension;
    int capacity;
    int numVehicles;
    int depotId;
    std::vector<Node> nodes;
    std::vector<int> demands;
    std::vector<std::vector<double>> distanceMatrix;
};


TEST(SolutionTest, BasicOperations) {
    VRPLIBReaderFake instancia;
    Solution sol(instancia);

    // Agregar ruta con cliente 2
    sol.addRuta(2);
    auto rutas = sol.getRutas();
    EXPECT_EQ(rutas.size(), 1);
    EXPECT_EQ(rutas[0], (std::vector<int>{1, 2, 1}));

    // Verificar demanda y distancia iniciales
    auto demandas = sol.getDemandas();
    EXPECT_EQ(demandas[0], 5); // demanda nodo 2

    auto dist = sol.getDistancias();
    EXPECT_DOUBLE_EQ(dist[0], 10.0); // 1->2 + 2->1 = 5 + 5

    // Insertar cliente 3 en posición 2 de ruta 0
    sol.addClient(3, 0, 2);
    rutas = sol.getRutas();
    EXPECT_EQ(rutas[0], (std::vector<int>{1, 2, 3, 1}));

    demandas = sol.getDemandas();
    EXPECT_EQ(demandas[0], 12); // 5 + 7

    dist = sol.getDistancias();
    // Distancia 1->2 (5) + 2->3 (5) + 3->1 (10) = 20
    EXPECT_DOUBLE_EQ(dist[0], 20.0);

    // Quitar cliente 2 de posición 1
    sol.removeClient(0, 1);
    rutas = sol.getRutas();
    EXPECT_EQ(rutas[0], (std::vector<int>{1, 3, 1}));

    demandas = sol.getDemandas();
    EXPECT_EQ(demandas[0], 7);

    dist = sol.getDistancias();
    // Distancia 1->3 + 3->1 = 10 + 10 = 20
    EXPECT_DOUBLE_EQ(dist[0], 20.0);

    // Comprobar validez (demanda <= capacidad)
    EXPECT_TRUE(sol.esValida(0));

    // Agregar cliente 2 otra vez, comprobar demanda y validez
    sol.addClient(2, 0, 1);
    demandas = sol.getDemandas();
    EXPECT_EQ(demandas[0], 12); // 7 + 5
    EXPECT_TRUE(sol.esValida(0));
}
