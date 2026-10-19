#ifndef ESTADOSPIDER_H
#define ESTADOSPIDER_H

#include "Carta.h"
#include "Lista.h"

// Guarda una copia completa de la partida para las acciones de deshacer y rehacer.
class EstadoSpider {
public:
	Lista<Carta>* columnas[10];   // [0] es la base de la columna y la última posición es el tope.
	Lista<Carta>* mazo;           // [0] es la próxima carta que se va a repartir.
	int reparticionesRestantes;   // Cantidad de repartos que le quedan al mazo.
	int escalerasCompletadas;     // Cantidad de escaleras K->A eliminadas.

	// Crea el estado vacío dejando en nulo las listas de columnas y del mazo.
	EstadoSpider() : mazo(nullptr), reparticionesRestantes(0), escalerasCompletadas(0) {
		for (int i = 0; i < 10; i++)
			columnas[i] = nullptr;
	}

	// Libera las listas de las 10 columnas y la del mazo.
	~EstadoSpider() {
		for (int i = 0; i < 10; i++)
			delete columnas[i];
		delete mazo;
	}

	// Impide copiar el estado porque es dueño de listas reservadas con new.
	EstadoSpider(const EstadoSpider&) = delete;
	void operator = (const EstadoSpider&) = delete;
};

#endif
