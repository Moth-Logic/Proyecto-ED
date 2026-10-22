#ifndef CURSOR_H
#define CURSOR_H

#include "JuegoSpider.h"

// Cursor del tablero: la columna señalada y, si hay agarre, de dónde salen las cartas y cuántas son.
// Las cartas agarradas siguen en el juego hasta que se sueltan, así que cancelar no cambia nada.
class CursorSpider {
private:
	int columna;
	bool agarrando;
	int origen;
	int cantidad;

public:
	CursorSpider() : columna(0), agarrando(false), origen(-1), cantidad(0) {}

	int getColumna() const { return columna; }
	bool estaAgarrando() const { return agarrando; }
	int getOrigen() const { return origen; }
	int getCantidad() const { return cantidad; }

	void izquierda() { if (columna > 0) columna--; }
	void derecha() { if (columna < 9) columna++; }

	// Cuántas cartas del tope forman una escalera que se puede agarrar (0 si no hay ninguna).
	// Se prueba de la escalera más larga hacia la más corta, así se toma el grupo máximo.
	static int grupoMovible(JuegoSpider& juego, int indice) {
		PilaCartaSpider& col = juego.getColumna(indice);
		int n = col.tamano();
		for (int k = n; k >= 1; k--)
			if (col.formaEscaleraAscendente(n - k))
				return k;
		return 0;
	}

	// Agarra el grupo movible más largo de la columna actual. Devuelve false si no hay nada que agarrar.
	bool agarrar(JuegoSpider& juego) {
		int k = grupoMovible(juego, columna);
		if (k == 0) return false;
		agarrando = true;
		origen = columna;
		cantidad = k;
		return true;
	}

	// Intenta soltar el grupo en la columna actual. Si el juego rechaza el movimiento, el agarre se conserva.
	bool soltar(JuegoSpider& juego) {
		if (!agarrando) return false;
		if (!juego.mover(origen + 1, columna + 1, cantidad)) return false;
		cancelar();
		return true;
	}

	// Suelta el agarre sin mover nada.
	void cancelar() {
		agarrando = false;
		origen = -1;
		cantidad = 0;
	}
};

#endif
