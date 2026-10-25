#include <iostream>
#include <windows.h>
#include <string>
#include <sstream>
#include "JuegoGolf.h"
#include "JuegoSpider.h"

using std::string;
using std::istringstream;
using std::cin;
using std::cout;
using std::endl;

// Pregunta al jugador si quiere jugar otra partida y valida la respuesta S o N.
bool repetir() {

    string respuesta;
    cout << endl << "¿Desea jugar de nuevo? (S/N): ";
    getline(cin, respuesta);

    while (respuesta != "s" && respuesta != "n" && respuesta != "S" && respuesta != "N") {
        system("cls");

        cout << "Respuesta inválida. Intente nuevamente." << endl;
        cout << "¿Desea jugar de nuevo? (S/N): ";
        getline(cin, respuesta);
    }

    cout << endl;
    system("cls");
    return respuesta == "s" || respuesta == "S";
}

// Fución genérica que limpia la pantalla y muestra el tablero cuando el jugador debe verlo.
template <typename TypeGame>
void actualizarPantalla(TypeGame& juego, bool mostrarJuego) {
    system("cls");

    if (mostrarJuego)
        cout << endl << juego.mostrarEstado() << endl << endl;
}

// Función genérica que lee un número entero desde el teclado y devuelve sigue = false cuando el jugador escribe Q o returna
// un mensaje de error cuando la entrada es inválida y actuliza la pantalla llamando a otra función.
template <typename TypeGame>
int leerEntrada(TypeGame& juego, bool& sigue, string mensaje) {
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

        actualizarPantalla(juego, true);

        cout << "(Presione [Q] para volver)\n\n";
        cout << "Valor inválido. Intente nuevamente." << endl;
    }
}

int main() {

    SetConsoleOutputCP(65001);

    bool continuar;
    continuar = true;
    while (continuar) {

        // Muestra el menú principal y valida la opción elegida.
        cout << "Qué quieres hacer?" << endl << "[1].Golf Solitaire" << endl << "[2].Spider Solitaire" << endl;
        string opcion;
        cout << "Seleccione una opción entre 1 y 2: ";
        getline(cin, opcion);

        while (opcion != "1" && opcion != "2") {
            cout << "Opción inválida. Intente nuevamente." << endl;
            cout << "Seleccione una opción entre 1 y 2: ";
            getline(cin, opcion);
        }
        cout << endl;

        // Ciclo principal del Golf: dibuja el tablero y pide una acción.
        if (opcion == "1") {
            JuegoGolf juego;
            juego.iniciarPartida();
            juego.borrarUltimoError();

            while (!juego.estaPartidaTerminada()) {

                actualizarPantalla(juego, true);

                cout << "Qué quieres hacer?" << endl << endl;

                cout << "[1].Mover" << endl << "[2].Repartir" << endl
                    << "[3].Deshacer" << endl << "[4].Rehacer" << endl << "[5].Abandonar" << endl << endl;

                if (juego.getUltimoError() != "") {
                    cout << juego.getUltimoError() << endl;
                }
                juego.borrarUltimoError();

                string accion;
                cout << "Seleccione una opción entre 1, 2, 3, 4 y 5: ";
                getline(cin, accion);

                while (accion != "1" && accion != "2" && accion != "3" && accion != "4" && accion != "5") {

                    actualizarPantalla(juego, true);

                    cout << "Qué quieres hacer?" << endl << endl;

                    cout << "[1].Mover" << endl << "[2].Repartir" << endl
                        << "[3].Deshacer" << endl << "[4].Rehacer" << endl << "[5].Abandonar" << endl << endl;

                    cout << "Opción inválida. Intente nuevamente." << endl;
                    cout << "Seleccione una opción entre 1, 2, 3, 4 y 5: ";
                    getline(cin, accion);
                }

                if (accion == "1") {

                    actualizarPantalla(juego, true);

                    bool movimientoRealizado = false;
                    bool seguir = true;
                    int origen;

                    juego.borrarUltimoError();

                    // Pide la columna de origen hasta que el movimiento se realice o el jugador salga con Q.
                    while (!movimientoRealizado && seguir) {

                        //Ingresando origen.
                        cout << "(Presione [Q] para volver)\n\n";

                        if (juego.getUltimoError() != "") {
                            cout << juego.getUltimoError() << endl;
                        }
                        juego.borrarUltimoError();

                        origen = leerEntrada(juego, seguir, "Ingrese la columna de donde desea tomar la carta: ");
                        actualizarPantalla(juego, true);

                        while (seguir && !juego.origenValido(origen)) {
                            cout << "(Presione [Q] para volver)\n\n";

                            if (juego.getUltimoError() != "") {
                                cout << juego.getUltimoError() << endl;
                            }
                            juego.borrarUltimoError();

                            origen = leerEntrada(juego, seguir, "Ingrese la columna de donde desea tomar la carta: ");
                            actualizarPantalla(juego, true);
                        }

                        if (seguir) {
                            movimientoRealizado = juego.mover(origen);

                            // Avisa el motivo cuando la regla del juego rechaza el movimiento.
                            if (!movimientoRealizado) {
                                actualizarPantalla(juego, true);
                            }
                        }
                    }
                }

                // Reparte una carta nueva en cada columna.
                else if (accion == "2") {
                    juego.repartir();
                }

                // Devuelve el estado anterior a la última jugada.
                else if (accion == "3") {
                    juego.deshacer();
                }

                // Vuelve a aplicar la última jugada deshecha.
                else if (accion == "4") {
                    juego.rehacer();
                }

                // Termina la partida actual y vuelve al menú principal.
                else if (accion == "5") {
                    juego.abandonar();
                }
            }

            actualizarPantalla(juego, false);

            if (juego.juegoGanado())
                cout << "¡Felicidades! Ganaste la partida de Golf." << endl;
            else
                cout << "Partida abandonada." << endl;
        }

        else if (opcion == "2") {
            JuegoSpider juego;
            juego.iniciarPartida();
            juego.borrarUltimoError();

            while (!juego.estaPartidaTerminada()) {

                actualizarPantalla(juego, true);

                cout << "Qué quieres hacer?" << endl << endl;

                cout << "[1].Mover" << endl << "[2].Repartir" << endl
                    << "[3].Deshacer" << endl << "[4].Rehacer" << endl << "[5].Abandonar" << endl << endl;

                if (juego.getUltimoError() != "") {
                    cout << juego.getUltimoError() << endl;
                }
                juego.borrarUltimoError();

                string accion;
                cout << "Seleccione una opción entre 1, 2, 3, 4 y 5: ";
                getline(cin, accion);

                while (accion != "1" && accion != "2" && accion != "3" && accion != "4" && accion != "5") {

                    actualizarPantalla(juego, true);

                    cout << "Qué quieres hacer?" << endl << endl;

                    cout << "[1].Mover" << endl << "[2].Repartir" << endl
                        << "[3].Deshacer" << endl << "[4].Rehacer" << endl << "[5].Abandonar" << endl << endl;

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

                    juego.borrarUltimoError();

                    while (!movimientoRealizado && seguir) {

                        //Ingresando origen.
                        cout << "(Presione [Q] para volver)\n\n";

                        if (juego.getUltimoError() != "") {
                            cout << juego.getUltimoError() << endl;
                        }
                        juego.borrarUltimoError();

                        origen = leerEntrada(juego, seguir, "Ingrese la columna de donde desea tomar: ");
                        actualizarPantalla(juego, true);

                        while (seguir && !juego.origenValido(origen)) {
                            cout << "(Presione [Q] para volver)\n\n";

                            if (juego.getUltimoError() != "") {
                                cout << juego.getUltimoError() << endl;
                            }
                            juego.borrarUltimoError();

                            origen = leerEntrada(juego, seguir, "Ingrese la columna de donde desea tomar: ");
                            actualizarPantalla(juego, true);
                        }

                        if (seguir) {
                            //Ingresando destino.
                            cout << "(Presione [Q] para volver)\n\n";
                            cout << "Coloumna de origen seleccionada: " << origen << "\n\n";

                            if (juego.getUltimoError() != "") {
                                cout << juego.getUltimoError() << endl;
                            }
                            juego.borrarUltimoError();

                            destino = leerEntrada(juego, seguir, "Ingrese la columna en donde desea colocar: ");
                            actualizarPantalla(juego, true);

                            while (seguir && !juego.destinoValido(origen, destino)) {
                                cout << "(Presione [Q] para volver)\n\n";
                                cout << "Coloumna de origen seleccionada: " << origen << "\n\n";

                                if (juego.getUltimoError() != "") {
                                    cout << juego.getUltimoError() << endl;
                                }
                                juego.borrarUltimoError();

                                destino = leerEntrada(juego, seguir, "Ingrese la columna en donde desea colocar: ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            //Ingresando cantidad.
                            cout << "(Presione [Q] para volver)\n\n";
                            cout << "Coloumna de origen seleccionada: " << origen << "\n";
                            cout << "Coloumna de destino seleccionada: " << destino << "\n\n";

                            if (juego.getUltimoError() != "") {
                                cout << juego.getUltimoError() << endl;
                            }
                            juego.borrarUltimoError();

                            cantidad = leerEntrada(juego, seguir, "Ingrese la cantidad de cartas que desea mover: ");
                            actualizarPantalla(juego, true);

                            while (seguir && !juego.cantidadValida(origen, cantidad)) {
                                cout << "(Presione [Q] para volver)\n\n";
                                cout << "Coloumna de origen seleccionada: " << origen << "\n";
                                cout << "Coloumna de destino seleccionada: " << destino << "\n\n";

                                if (juego.getUltimoError() != "") {
                                    cout << juego.getUltimoError() << endl;
                                }
                                juego.borrarUltimoError();

                                cantidad = leerEntrada(juego, seguir, "Ingrese la cantidad de cartas que desea mover: ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            movimientoRealizado = juego.mover(origen, destino, cantidad);

                            if (!movimientoRealizado) {
                                actualizarPantalla(juego, true);
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

            if (juego.juegoGanado())
                cout << "¡Felicidades! Ganaste la partida de Spider." << endl;
            else
                cout << "Partida abandonada." << endl;
        }
        continuar = repetir();
    }

    return 0;
}
