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
    sol.addClient(9, 0,38, 1);
    sol.addClient(30, 0, 9, 1);
    sol.addClient(34, 0, 30, 1);
    sol.addClient(50, 0, 34, 1);
    sol.addClient(16, 0, 50, 1);
    sol.addClient(21, 0, 16, 1);
    sol.addClient(29, 0, 21, 1);
    sol.addClient(2, 0, 29, 1);
    sol.addClient(11, 0, 2, 1);

    // RUTA 2
    sol.addRuta(32);
    sol.addClient(1, 1, 32, 1);
    sol.addClient(22, 1, 1, 1);
    sol.addClient(20, 1, 22, 1);
    sol.addClient(35, 1, 20, 1);
    sol.addClient(36, 1, 35, 1);
    sol.addClient(3, 1, 36, 1);
    sol.addClient(28, 1, 3, 1);
    sol.addClient(31, 1, 28,1);
    sol.addClient(26, 1, 31, 1);
    sol.addClient(8, 1, 26, 1);

    // RUTA 3
    sol.addRuta(27);
    sol.addClient(48, 2, 27, 1);
    sol.addClient(23, 2, 48, 1);
    sol.addClient(7, 2, 23, 1);
    sol.addClient(43, 2, 7, 1);
    sol.addClient(24, 2, 43, 1);
    sol.addClient(25, 2, 24, 1);
    sol.addClient(14, 2, 25, 1);
    sol.addClient(6, 2, 14, 1);

    // RUTA 4
    sol.addRuta(18);
    sol.addClient(13, 3, 18, 1);
    sol.addClient(41, 3, 13, 1);
    sol.addClient(40, 3, 41, 1);
    sol.addClient(19, 3, 40, 1);
    sol.addClient(42, 3, 19, 1);
    sol.addClient(17, 3, 42, 1);
    sol.addClient(4, 3, 17, 1);
    sol.addClient(47, 3, 4, 1);

    // RUTA 5
    sol.addRuta(12);
    sol.addClient(37, 4, 12, 1);
    sol.addClient(44, 4, 37, 1);
    sol.addClient(15, 4, 44, 1);
    sol.addClient(45, 4, 15, 1);
    sol.addClient(33, 4, 45, 1);
    sol.addClient(39, 4, 33, 1);
    sol.addClient(10, 4, 39, 1);
    sol.addClient(49, 4, 10, 1);
    sol.addClient(5, 4, 49, 1);
    sol.addClient(46, 4, 5, 1);

    // Imprimir solución
    sol.printSolution();
    vector<vector<int>> rutas= sol.getRutas();
     for (int i = 0; i < rutas.size(); ++i) {
        std::cout << "Ruta " << i + 1 << ": ";
        for (int nodo : rutas[i]) {
            std::cout << nodo << " ";
        }
        std::cout << std::endl;

    }
        std::string archivo = "instancias/E151-12c.dat";

 

    // Crear solución
    VRPLIBReader instanci(archivo);
    Solution sol1(instanci );
    sol1.addRuta(108);
  
    sol1.addClient(18, 0, 108, 1);
    sol1.addClient(103,0, 18,1);
    sol1.removeClient(18, 0, 108, 103);
    sol1.addClient(12, 0, 108, 1);


    // Imprimir solución
    sol1.printSolution();
    return 0;
}

