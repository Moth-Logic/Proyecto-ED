#ifndef CONSOLA_H
#define CONSOLA_H

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <conio.h>
#include <cctype>
#include <cstdio>
#include <string>

// Teclas que usa el juego. Las flechas en Windows llegan como dos códigos (0 o 0xE0 y luego un número).
enum class Tecla { Ninguna, Izquierda, Derecha, Enter, Espacio, Escape, Letra, Otra };

struct Entrada {
	Tecla tecla = Tecla::Ninguna;
	char letra = 0;   // se usa solo cuando la tecla es una letra, en mayúscula
};

// Activa UTF-8 y los códigos ANSI de color en la consola de Windows.
inline void iniciarConsola() {
	SetConsoleOutputCP(65001);
	HANDLE salida = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD modo = 0;
	if (GetConsoleMode(salida, &modo))
		SetConsoleMode(salida, modo | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

// Oculta y muestra el cursor parpadeante de la consola.
inline void ocultarCursor() { std::fputs("\x1b[?25l", stdout); std::fflush(stdout); }
inline void mostrarCursor() { std::fputs("\x1b[?25h\x1b[0m", stdout); std::fflush(stdout); }

// Limpia la pantalla y deja el cursor arriba a la izquierda.
inline void limpiarPantalla() { std::fputs("\x1b[2J\x1b[H", stdout); std::fflush(stdout); }

// Pinta un cuadro completo desde la esquina superior izquierda, sin parpadeo.
inline void dibujar(const std::string& cuadro) {
	std::fputs("\x1b[H", stdout);
	std::fputs(cuadro.c_str(), stdout);
	std::fputs("\x1b[0J", stdout);
	std::fflush(stdout);
}

inline void esperar(int ms) { Sleep(ms); }

// Descarta todas las teclas que estén pendientes.
inline void vaciarTeclado() {
	while (_kbhit()) _getch();
}

// Lee una tecla sin bloquear. Devuelve Ninguna si no hay nada pendiente.
inline Entrada leerEntrada() {
	Entrada e;
	if (!_kbhit()) return e;

	int c = _getch();
	if (c == 0 || c == 0xE0) {
		int f = _getch();
		e.tecla = Tecla::Otra;
		if (f == 75) e.tecla = Tecla::Izquierda;
		else if (f == 77) e.tecla = Tecla::Derecha;
		return e;
	}
	if (c == 13) e.tecla = Tecla::Enter;
	else if (c == 32) e.tecla = Tecla::Espacio;
	else if (c == 27) e.tecla = Tecla::Escape;
	else if (std::isalpha(c)) {
		e.tecla = Tecla::Letra;
		e.letra = static_cast<char>(std::toupper(c));
	}
	else e.tecla = Tecla::Otra;
	return e;
}

#endif
