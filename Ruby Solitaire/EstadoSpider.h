#ifndef ESTADOSPIDER_H
#define ESTADOSPIDER_H

#include "Carta.h"
#include "Lista.h"

// Snapshot del estado completo, usado por deshacer/rehacer. Guarda punteros
// a Listas (new) porque las estructuras del curso no son copiables; el
// destructor las libera. Por eso la copia está deshabilitada.
class EstadoSpider {
public:
	Lista<Carta>* columnas[10];   // [0] = base de la columna, último = tope
	Lista<Carta>* mazo;           // [0] = próxima carta a repartir
	int reparticionesRestantes;
	int escalerasCompletadas;

	EstadoSpider() : mazo(nullptr), reparticionesRestantes(0), escalerasCompletadas(0) {
		for (int i = 0; i < 10; i++)
			columnas[i] = nullptr;
	}

	~EstadoSpider() {
		for (int i = 0; i < 10; i++)
			delete columnas[i];
		delete mazo;
	}

	EstadoSpider(const EstadoSpider&) = delete;
	void operator = (const EstadoSpider&) = delete;
};

#endif
