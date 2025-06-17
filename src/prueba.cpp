#include <iostream>
#include "VRPLIBReader.h"
#include "Solution.h"


int main() {
    // Cambia esta ruta al archivo VRP que tengas
    std::string archivoVRP = "instancias/E051-05e.dat";

    // Leer instancia
    VRPLIBReader instancia(archivoVRP);

    // Crear solución

    Solution sol(instancia);

    // RUTA 1
    sol.addRuta(38);
    sol.addClient(9, 0, 0, 38);
    sol.addClient(30, 0, 9, 38);
    sol.addClient(34, 0, 30, 38);
    sol.addClient(50, 0, 34, 38);
    sol.addClient(16, 0, 50, 38);
    sol.addClient(21, 0, 16, 38);
    sol.addClient(29, 0, 21, 38);
    sol.addClient(2, 0, 29, 38);
    sol.addClient(11, 0, 2, 38);

    // RUTA 2
    sol.addRuta(32);
    sol.addClient(1, 1, 0, 32);
    sol.addClient(22, 1, 1, 32);
    sol.addClient(20, 1, 22, 32);
    sol.addClient(35, 1, 20, 32);
    sol.addClient(36, 1, 35, 32);
    sol.addClient(3, 1, 36, 32);
    sol.addClient(28, 1, 3, 32);
    sol.addClient(31, 1, 28, 32);
    sol.addClient(26, 1, 31, 32);
    sol.addClient(8, 1, 26, 32);

    // RUTA 3
    sol.addRuta(27);
    sol.addClient(48, 2, 0, 27);
    sol.addClient(23, 2, 48, 27);
    sol.addClient(7, 2, 23, 27);
    sol.addClient(43, 2, 7, 27);
    sol.addClient(24, 2, 43, 27);
    sol.addClient(25, 2, 24, 27);
    sol.addClient(14, 2, 25, 27);
    sol.addClient(6, 2, 14, 27);

    // RUTA 4
    sol.addRuta(18);
    sol.addClient(13, 3, 0, 18);
    sol.addClient(41, 3, 13, 18);
    sol.addClient(40, 3, 41, 18);
    sol.addClient(19, 3, 40, 18);
    sol.addClient(42, 3, 19, 18);
    sol.addClient(17, 3, 42, 18);
    sol.addClient(4, 3, 17, 18);
    sol.addClient(47, 3, 4, 18);

    // RUTA 5
    sol.addRuta(12);
    sol.addClient(37, 4, 0, 12);
    sol.addClient(44, 4, 37, 12);
    sol.addClient(15, 4, 44, 12);
    sol.addClient(45, 4, 15, 12);
    sol.addClient(33, 4, 45, 12);
    sol.addClient(39, 4, 33, 12);
    sol.addClient(10, 4, 39, 12);
    sol.addClient(49, 4, 10, 12);
    sol.addClient(5, 4, 49, 12);
    sol.addClient(46, 4, 5, 12);

    // Imprimir solución
    sol.printSolution();


    return 0;
}

