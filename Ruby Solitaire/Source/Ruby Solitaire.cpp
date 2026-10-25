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
        cout << "Respuesta inválida. Intente nuevamente." << endl;
        cout << "¿Desea jugar de nuevo? (S/N): ";
        getline(cin, respuesta);
    }

    cout << endl;
    return respuesta == "s" || respuesta == "S";
}

// Limpia la pantalla y muestra el tablero cuando el jugador debe verlo.
void actualizarPantalla(JuegoSpider& juego, bool mostrarJuego) {
    system("cls");

    if (mostrarJuego)
        cout << endl << juego.mostrarEstado() << endl << endl;
}

// Limpia la pantalla y muestra el tablero del Golf con el formato de texto básico.
void actualizarPantallaGolf(JuegoGolf& juego, bool mostrarJuego) {
    system("cls");

    if (mostrarJuego)
        cout << endl << juego.mostrarEstado() << endl;
}

// Lee un número entero desde el teclado y devuelve sigue = false cuando el jugador escribe Q.
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

            while (!juego.estaPartidaTerminada()) {

                actualizarPantallaGolf(juego, true);

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

                    bool movimientoRealizado = false;
                    bool seguir = true;

                    // Pide la columna de origen hasta que el movimiento se realice o el jugador salga con Q.
                    while (!movimientoRealizado && seguir) {

                        int origen = leerEntrada(seguir, "Ingrese la columna de origen (Q para volver): ");
                        actualizarPantallaGolf(juego, true);

                        if (seguir) {
                            movimientoRealizado = juego.mover(origen);

                            // Avisa el motivo cuando la regla del juego rechaza el movimiento.
                            if (!movimientoRealizado) {
                                actualizarPantallaGolf(juego, true);
                                cout << juego.getUltimoError() << endl;
                            }
                        }
                    }
                }

                // Reparte una carta nueva en cada columna.
                else if (accion == "2") {
                    if (!juego.repartir()) {
                        actualizarPantallaGolf(juego, true);
                        cout << juego.getUltimoError() << endl;
                    }
                }

                // Devuelve el estado anterior a la última jugada.
                else if (accion == "3") {
                    if (!juego.deshacer()) {
                        actualizarPantallaGolf(juego, true);
                        cout << juego.getUltimoError() << endl;
                    }
                }

                // Vuelve a aplicar la última jugada deshecha.
                else if (accion == "4") {
                    if (!juego.rehacer()) {
                        actualizarPantallaGolf(juego, true);
                        cout << juego.getUltimoError() << endl;
                    }
                }

                // Termina la partida actual y vuelve al menú principal.
                else if (accion == "5") {
                    juego.abandonar();
                }
            }

            actualizarPantallaGolf(juego, false);

            if (juego.juegoGanado())
                cout << "¡Felicidades! Ganaste la partida de Golf." << endl;
            else
                cout << "Partida abandonada." << endl;
        }

        else if (opcion == "2") {
            JuegoSpider juego;
            juego.iniciarPartida();

            // Ciclo principal de la partida: dibuja el tablero y pide una acción.
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

                    // Pide origen, destino y cantidad hasta que el movimiento se realice o el jugador salga con Q.
                    while (!movimientoRealizado && seguir) {

                        origen = leerEntrada(seguir, "Ingrese la columna de origen (Q para volver): ");
                        actualizarPantalla(juego, true);

                        // Repite la lectura del origen mientras la columna no sea válida.
                        while (seguir && !juego.origenValido(origen)) {
                            cout << juego.getUltimoError() << endl;

                            origen = leerEntrada(seguir, "Ingrese la columna de origen (Q para volver): ");
                            actualizarPantalla(juego, true);
                        }

                        if (seguir) {
                            destino = leerEntrada(seguir, "Ingrese la columna de destino (Q para volver): ");
                            actualizarPantalla(juego, true);

                            // Repite la lectura del destino mientras la columna no sea válida.
                            while (seguir && !juego.destinoValido(origen, destino)) {
                                cout << juego.getUltimoError() << endl;

                                destino = leerEntrada(seguir, "Ingrese la columna de destino (Q para volver): ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            cantidad = leerEntrada(seguir, "Ingrese la cantidad de cartas (Q para volver): ");
                            actualizarPantalla(juego, true);

                            // Repite la lectura de la cantidad mientras no quepa en la columna de origen.
                            while (seguir && !juego.cantidadValida(origen, cantidad)) {
                                cout << juego.getUltimoError() << endl;

                                cantidad = leerEntrada(seguir, "Ingrese la cantidad de cartas (Q para volver): ");
                                actualizarPantalla(juego, true);
                            }
                        }

                        if (seguir) {
                            movimientoRealizado = juego.mover(origen, destino, cantidad);

                            // Avisa el motivo cuando la regla del juego rechaza el movimiento.
                            if (!movimientoRealizado) {
                                actualizarPantalla(juego, true);
                                cout << juego.getUltimoError() << endl;
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
                cout << "¡Felicidades! Ganaste la partida de Spider." << endl;
            else
                cout << "Partida abandonada." << endl;
        }
        continuar = repetir();
    }

	return 0;
}
