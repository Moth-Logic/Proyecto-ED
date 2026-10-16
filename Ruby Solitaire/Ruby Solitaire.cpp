#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include "JuegoSpider.h"

using std::string;
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;

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

void actualizarPantalla(JuegoSpider& juego, bool mostrarJuego) {
    system("cls");

    if (mostrarJuego)
        cout << endl << juego.mostrarEstado() << endl;
}

int leerEntrada(bool& sigue, string mensaje) {
    string entrada;
    int numero;
    char extra;

    while (true) {
        cout << mensaje;
        getline(cin, entrada);

        if (entrada == "q" || entrada == "Q") {
            sigue = false;
            return 0;
        }

        istringstream leer(entrada);

        if (leer >> numero && !(leer >> extra)) {
            sigue = true;
            return numero;
        }

        cout << "Valor inválido. Intente nuevamente." << endl;
    }
}

int main() {

    SetConsoleOutputCP(65001);

    bool continuar;
    continuar = true;
    while (continuar) {

        cout << "Qué quieres hacer?" << endl << "[1].Spider Solitaire" << endl << "[2].Juego 2" << endl;
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
            JuegoSpider juego;
            juego.iniciarPartida();

            while (!juego.estaPartidaTerminada()) {

                actualizarPantalla(juego, true);

                cout << "Qué quieres hacer?" << endl << "[1].Mover" << endl << "[2].Repartir" << endl
                    << "[3].Deshacer" << endl << "[4].Rehacer" << endl << "[5].Abandonar" << endl;
                string accion;
                cout << "Seleccione una opción entre 1, 2, 3, 4 y 5: ";
                getline(cin, accion);

                while (accion != "1" && accion != "2" && accion != "3" && accion != "4" && accion != "5") {
                    cout << "Opción inválida. Intente nuevamente." << endl;
                    cout << "Seleccione una opción entre 1, 2, 3, 4 y 5: ";
                    getline(cin, accion);
                }

                if (accion == "1") {

                    actualizarPantalla(juego, true);

                    bool movimientoRealizado = false;
                    bool seguir = true;
                    int origen = 0;
                    int destino = 0;
                    int cantidad = 0;

                    while (!movimientoRealizado && seguir) {

                        //Ingresando origen.
                        origen = leerEntrada(seguir, "Ingrese la columna de origen (Q para volver): ");
                        actualizarPantalla(juego, true);

                        while (seguir && !juego.origenValido(origen)) {
                            cout << juego.getUltimoError() << endl;

                            origen = leerEntrada(seguir, "Ingrese la columna de origen (Q para volver): ");
                            actualizarPantalla(juego, true);
                        }

                        if (seguir) {
                            //Ingresando destino.
                            destino = leerEntrada(seguir, "Ingrese la columna de destino (Q para volver): ");
                            actualizarPantalla(juego, true);

                            while (seguir && !juego.destinoValido(origen, destino)) {
                                cout << juego.getUltimoError() << endl;

                                destino = leerEntrada(seguir, "Ingrese la columna de destino (Q para volver): ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            //Ingresando cantidad.
                            cantidad = leerEntrada(seguir, "Ingrese la cantidad de cartas (Q para volver): ");
                            actualizarPantalla(juego, true);

                            while (seguir && !juego.cantidadValida(origen, cantidad)) {
                                cout << juego.getUltimoError() << endl;

                                cantidad = leerEntrada(seguir, "Ingrese la cantidad de cartas (Q para volver): ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            movimientoRealizado = juego.mover(origen, destino, cantidad);

                            if (!movimientoRealizado) {
                                actualizarPantalla(juego, true);
                                cout << juego.getUltimoError() << endl;
                            }
                        }
                    }
                }

                else if (accion == "2") {
                    juego.repartir();
                }

                else if (accion == "3") {
                    juego.deshacer();
                }

                else if (accion == "4") {
                    juego.rehacer();
                }

                else if (accion == "5") {
                    juego.abandonar();
                }
            }

            actualizarPantalla(juego, false);
        }
        else if (opcion == "2") {
            cout << "Holiwiii mrweep!!! UwU" << endl;
        }
        continuar = repetir();
    }

    return 0;
}
