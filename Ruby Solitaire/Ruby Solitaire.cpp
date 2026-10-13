#include <iostream>
#include "JuegoSpider.h"

int main() {
	JuegoSpider juego;
	juego.iniciarPartida();
	std::cout << juego.mostrarEstado() << std::endl;
	return 0;
}
