#ifndef RENDER_H
#define RENDER_H

#include <string>
#include <vector>
#include "JuegoSpider.h"
#include "Cursor.h"
#include "Lienzo.h"

// Medidas del tablero. Cada carta ocupa 9 columnas y dejamos 2 de separación.
const int ANCHO_COLUMNA = 11;
const int MARGEN_IZQ = 4;
const int ANCHO_PANTALLA = 118;
const int ALTO_CARTA_MIN = 6;   // una carta completa ocupa 6 líneas
const int FILA_TABLERO = 4;     // fila donde empiezan las cartas

// Posición horizontal de una columna (índice 0 a 9).
inline int colX(int indice) { return MARGEN_IZQ + indice * ANCHO_COLUMNA; }

// Efectos temporales de la interfaz: mensajes, destellos y cartas en vuelo.
// Todo se mide en milisegundos y se descuenta con tick().
struct Efectos {
	std::string mensaje;
	Color colorMensaje = Color::Normal;
	int msMensaje = 0;

	int msError = 0;          // destello rojo de la mano tras un movimiento inválido
	int msPulso = 0;          // destello de la columna que recibió cartas
	int columnaPulso = -1;

	std::vector<std::string> vuelo;   // líneas de las cartas que se están moviendo
	int xIni = 0;
	int xFin = 0;
	int msVuelo = 0;
	int msVueloTotal = 1;

	// Muestra un mensaje debajo del tablero durante un tiempo.
	void avisar(const std::string& texto, Color color, int duracion = 2500) {
		mensaje = texto;
		colorMensaje = color;
		msMensaje = duracion;
	}

	// Hace que las cartas se deslicen de una columna a otra.
	void volar(const std::vector<std::string>& lineas, int desde, int hasta, int duracion) {
		vuelo = lineas;
		xIni = desde;
		xFin = hasta;
		msVuelo = duracion;
		msVueloTotal = duracion;
	}

	// Resalta una columna durante un momento.
	void pulsar(int columna, int duracion) {
		columnaPulso = columna;
		msPulso = duracion;
	}

	// Descuenta el tiempo transcurrido de cada efecto.
	void tick(int ms) {
		if (msMensaje > 0) msMensaje = (msMensaje > ms) ? msMensaje - ms : 0;
		if (msError > 0) msError = (msError > ms) ? msError - ms : 0;
		if (msPulso > 0) msPulso = (msPulso > ms) ? msPulso - ms : 0;
		if (msVuelo > 0) msVuelo = (msVuelo > ms) ? msVuelo - ms : 0;
		if (msVuelo == 0) vuelo.clear();
	}
};

// Separa un texto en líneas según los saltos de línea.
inline std::vector<std::string> separarLineas(const std::string& texto) {
	std::vector<std::string> lineas;
	if (texto.empty()) return lineas;
	std::string actual;
	for (char c : texto) {
		if (c == '\n') {
			lineas.push_back(actual);
			actual.clear();
		}
		else {
			actual += c;
		}
	}
	lineas.push_back(actual);
	return lineas;
}

// Arma las líneas de las cartas de una columna entre las posiciones 'desde' (incluida) y 'hasta' (excluida).
// Se usa una copia temporal para no tocar la columna real, así se puede dibujar parte de una pila.
inline std::vector<std::string> lineasDeRango(JuegoSpider& juego, int indice, int desde, int hasta) {
	PilaCartaSpider& original = juego.getColumna(indice);
	PilaCartaSpider copia;
	for (int k = desde; k < hasta; k++)
		copia.push(original[k]);
	return separarLineas(copia.mostrar());
}

// Dibuja un panel con borde redondeado y un título, al estilo de btop.
inline void dibujarPanel(Lienzo& L, int y0, int y1, const std::string& titulo) {
	int ancho = L.getAncho();
	for (int x = 1; x < ancho - 1; x++) {
		L.poner(x, y0, "─", Color::Gris);
		L.poner(x, y1, "─", Color::Gris);
	}
	for (int y = y0 + 1; y < y1; y++) {
		L.poner(0, y, "│", Color::Gris);
		L.poner(ancho - 1, y, "│", Color::Gris);
	}
	L.poner(0, y0, "╭", Color::Gris);
	L.poner(ancho - 1, y0, "╮", Color::Gris);
	L.poner(0, y1, "╰", Color::Gris);
	L.poner(ancho - 1, y1, "╯", Color::Gris);
	L.escribir(2, y0, "┤ " + titulo + " ├", Color::Cian);
}

// Arma la pantalla completa de la partida: barra superior, tablero, cursor, carta en mano y mensajes.
inline Lienzo renderSpider(JuegoSpider& juego, const CursorSpider& cursor, const Efectos& ef) {
	const int c = cursor.getColumna();

	// Líneas de cada columna. Si hay agarre, la columna de origen no muestra las cartas agarradas.
	std::vector<std::string> columnas[10];
	int altoTablero = ALTO_CARTA_MIN;
	for (int i = 0; i < 10; i++) {
		int total = juego.getColumna(i).tamano();
		int hasta = total;
		if (cursor.estaAgarrando() && cursor.getOrigen() == i)
			hasta = total - cursor.getCantidad();
		columnas[i] = lineasDeRango(juego, i, 0, hasta);
		if ((int)columnas[i].size() > altoTablero)
			altoTablero = (int)columnas[i].size();
	}

	// Las cartas agarradas se dibujan en la mano, así que también cuentan para el alto del tablero.
	std::vector<std::string> mano;
	if (cursor.estaAgarrando()) {
		int total = juego.getColumna(cursor.getOrigen()).tamano();
		mano = lineasDeRango(juego, cursor.getOrigen(), total - cursor.getCantidad(), total);
		if ((int)mano.size() > altoTablero)
			altoTablero = (int)mano.size();
	}

	const int yBordeTablero = FILA_TABLERO + altoTablero;
	const int yEstado = yBordeTablero + 1;
	const int alto = yEstado + 5;
	Lienzo L(ANCHO_PANTALLA, alto);

	// Barra superior con el nombre del juego y los contadores.
	L.escribir(2, 0, "◆ RUBY SOLITAIRE · SPIDER", Color::Cian);
	std::string stats = "Mazo " + std::to_string(juego.getCartasMazo())
		+ "  ·  Repartos " + std::to_string(juego.getReparticionesRestantes())
		+ "  ·  Escaleras " + std::to_string(juego.getEscalerasCompletadas()) + "/8";
	int anchoStats = (int)Lienzo::simbolos(stats).size();
	L.escribir(ANCHO_PANTALLA - 2 - anchoStats, 0, stats, Color::Gris);

	dibujarPanel(L, 1, yBordeTablero, "Board");

	// Flecha del cursor y números de columna. La columna seleccionada se resalta.
	for (int i = 0; i < 10; i++) {
		Color color = Color::Gris;
		if (i == c) color = (ef.msError > 0) ? Color::Rojo : Color::Amarillo;
		L.escribir(colX(i), 3, std::to_string(i + 1), color);
	}
	L.poner(colX(c) + 4, 2, "▼", (ef.msError > 0) ? Color::Rojo : Color::Amarillo);

	// Cartas de cada columna. La columna que acaba de recibir cartas se pinta de cian un momento.
	for (int i = 0; i < 10; i++) {
		Color base = (i == ef.columnaPulso && ef.msPulso > 0) ? Color::Cian : Color::Normal;
		for (size_t k = 0; k < columnas[i].size(); k++)
			L.escribir(colX(i), FILA_TABLERO + (int)k, columnas[i][k], base);
	}

	// Carta en vuelo: se desliza desde la columna de origen hasta la de destino.
	if (ef.msVuelo > 0 && !ef.vuelo.empty()) {
		double progreso = 1.0 - (double)ef.msVuelo / ef.msVueloTotal;
		int x = ef.xIni + (int)((ef.xFin - ef.xIni) * progreso);
		for (size_t k = 0; k < ef.vuelo.size(); k++)
			L.escribir(x, FILA_TABLERO + (int)k, ef.vuelo[k], Color::Blanco);
	}

	// Cartas agarradas: flotan sobre la columna donde está el cursor.
	if (cursor.estaAgarrando()) {
		Color colorMano = (ef.msError > 0) ? Color::Rojo : Color::Amarillo;
		for (size_t k = 0; k < mano.size(); k++)
			L.escribir(colX(c), FILA_TABLERO + (int)k, mano[k], colorMano);
	}

	// Panel de estado: mensaje y controles.
	dibujarPanel(L, yEstado, yEstado + 4, "Status");

	std::string linea;
	Color colorLinea = Color::Gris;
	if (ef.msMensaje > 0) {
		linea = ef.mensaje;
		colorLinea = ef.colorMensaje;
	}
	else if (cursor.estaAgarrando()) {
		linea = "Holding " + std::to_string(cursor.getCantidad()) + " card(s) from column "
			+ std::to_string(cursor.getOrigen() + 1) + ". Move with ←/→ and press ENTER to drop.";
		colorLinea = Color::Cian;
	}
	else {
		linea = "Use ←/→ to pick a column, then press SPACE to grab its top run.";
	}
	L.escribir(3, yEstado + 1, linea, colorLinea);
	L.escribir(3, yEstado + 2, "←/→ move cursor    SPACE grab / release    ENTER drop    ESC cancel");
	L.escribir(3, yEstado + 3, "R deal    U undo    Y redo    Q quit to menu");

	return L;
}

#endif
