#include <iostream>
#include <windows.h>
#include <string>
#include "JuegoSpider.h"

using std::string;
using std::cin;
using std::cout;
using std::endl;

int main() {

    SetConsoleOutputCP(65001);

	JuegoSpider juego;
	juego.iniciarPartida();
	cout << juego.mostrarEstado() << endl;

    bool continuar;
    continuar = true;
    while (continuar) {

        cout << "Qué quieres hacer?" << endl << "[1].Mover" << endl << "[2].Deshacer" << endl << "[3].Abandonar" << endl;
        string opcion;
        cout << "Seleccione una opción entre 1, 2 y 3: ";
        getline(cin, opcion);

        while (opcion != "1" && opcion != "2" && opcion != "3") {
            cout << "Opción inválida. Intente nuevamente." << endl;
            cout << "Seleccione una opción entre 1, 2 y 3: ";
            getline(cin, opcion);
        }
        cout << endl;
        if (opcion == "1") {
            cout << "holiwi" << endl;
        }
        else if (opcion == "2") {
            cout << "holiwi X2" << endl;
        }
        else if (opcion == "3") {
            juego.abandonar();
        }

    }

	return 0;
}
