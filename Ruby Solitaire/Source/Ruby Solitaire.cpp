#include <iostream>
#include <string>
#include "../Headers/Consola.h"
#include "../Headers/PantallaSpider.h"
#include <sstream>
#include "JuegoGolf.h"
#include "JuegoSpider.h"
#include "../Headers/Consola.h"
#include "../Headers/PantallaSpider.h"
#include <iostream>
#include <string>

using std::string;
using std::cin;
using std::cout;
using std::endl;

// Pregunta al jugador si quiere jugar otra partida y valida la respuesta S o N.
bool repetir() {
    string respuesta;
    cout << endl << "¿Desea jugar de nuevo? (S/N): ";
    getline(cin, respuesta);

    while (respuesta != "s" && respuesta != "n" && respuesta != "S" && respuesta != "N") {
        cout << "Respuesta inválida. Intente nuevamente." << endl;
        cout << "¿Desea jugar de nuevo? (S/N): ";
        getline(cin, respuesta);
    }

    cout << endl;
    return respuesta == "s" || respuesta == "S";
}

int main() {
    iniciarConsola();

    bool continuar = true;
    while (continuar) {

        // Muestra el menú principal y valida la opción elegida.
        cout << "Qué quieres hacer?" << endl << "[1].Juego 1" << endl << "[2].Spider Solitaire" << endl;
        string opcion;
        cout << "Seleccione una opción entre 1 y 2: ";
        getline(cin, opcion);

        while (opcion != "1" && opcion != "2") {
            cout << "Opción inválida. Intente nuevamente." << endl;
            cout << "Seleccione una opción entre 1 y 2: ";
            getline(cin, opcion);
        }
        cout << endl;

        if (opcion == "1") {
            cout << "Holiwiii mrweep!!! UwU" << endl;
        }
        else if (opcion == "2") {
            // La partida usa teclado directo (flechas, Space, Enter) y se controla dentro de jugarSpider().
            jugarSpider();
        }
        continuar = repetir();
    }

    mostrarCursor();
    return 0;
}
