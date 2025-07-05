#include <gtest/gtest.h>
#include "Solution.h"
#include "VRPLIBReader.h"

#include "Solution.h"
#include "VRPLIBReader.h"
#include <cassert>
#include <iostream>

using namespace std;

class SolutionTest : public ::testing::Test {
protected:
    VRPLIBReader instancia;

    SolutionTest() : instancia("instancias/E045-04f.dat") {} // asegurate que el archivo exista
};

TEST_F(SolutionTest, AddRuta) {
    Solution sol(instancia);
    int clientId = 2;

    sol.addRuta(clientId);

    auto rutas = sol.getRutas();
    ASSERT_EQ(rutas.size(), 1);
    EXPECT_EQ(rutas[0].front(), 1);  // depot
    EXPECT_EQ(rutas[0].back(), 1);   // depot
    EXPECT_EQ(rutas[0][1], clientId);

    auto demandas = sol.getDemandas();
    ASSERT_EQ(demandas[0], instancia.getDemands()[clientId]);

    auto distancias = sol.getDistancias();
    ASSERT_GT(distancias[0], 0); // debe ser mayor a 0
}

TEST_F(SolutionTest, AddAndRemoveClient) {
    Solution sol(instancia);

    int id1 = 2, id2 = 3;
    sol.addRuta(id1);
    sol.addClient(id2, 0, 2); // insertar antes del depot final

    auto rutas = sol.getRutas();
    ASSERT_EQ(rutas[0].size(), 4); // [1, id1, id2, 1]
    EXPECT_EQ(rutas[0][2], id2);

    auto demandas = sol.getDemandas();
    ASSERT_EQ(demandas[0], instancia.getDemands()[id1] + instancia.getDemands()[id2]);

    // Ahora removemos el cliente id2
    sol.removeClient(0, 2);
    rutas = sol.getRutas();
    ASSERT_EQ(rutas[0].size(), 3); // [1, id1, 1]
    EXPECT_EQ(rutas[0][1], id1);

    demandas = sol.getDemandas();
    ASSERT_EQ(demandas[0], instancia.getDemands()[id1]);
}

TEST_F(SolutionTest, EsValida) {
    Solution sol(instancia);
    sol.addRuta(2);

    bool valida = sol.esValida(0);
    ASSERT_TRUE(valida);
}

TEST_F(SolutionTest, Getters) {
    Solution sol(instancia);
    sol.addRuta(2);

    auto rutas = sol.getRutas();
    ASSERT_FALSE(rutas.empty());

    auto demandas = sol.getDemandas();
    ASSERT_FALSE(demandas.empty());

    auto distancias = sol.getDistancias();
    ASSERT_FALSE(distancias.empty());

    auto inst = sol.getInstancia();
    ASSERT_EQ(inst.getName(), instancia.getName());
}

TEST_F(SolutionTest, PrintSolution) {
    Solution sol(instancia);
    sol.addRuta(2);

    sol.printSolution();
}