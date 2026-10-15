#include <iostream>
#include "JuegoSpider.h"

using std::cin;
using std::cout;
using std::endl;

int main() {
	JuegoSpider juego;
	juego.iniciarPartida();
	cout << juego.mostrarEstado() << endl;

	return 0;
}
