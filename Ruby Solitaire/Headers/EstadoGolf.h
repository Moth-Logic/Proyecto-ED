#ifndef ESTADOGOLF_H
#define ESTADOGOLF_H

#include "Carta.h"
#include "Lista.h"

// Guarda una copia completa de la partida de Golf para las acciones de deshacer
// y rehacer, igual que EstadoSpider. Es dueño de las listas que reserva.
class EstadoGolf {
public:
	Lista<Carta>* columnas[7];   // [0] es la base de la columna y la última posición es el tope.
	Lista<Carta>* descarte;      // [0] es la primera carta jugada y la última posición es el tope.
	Lista<Carta>* mazo;          // [0] es el fondo del mazo y la última posición es la próxima a repartir.
	int puntuacion;              // Cartas que se habían pasado al descarte.
	int rondas;                  // Repartos que se habían dado en ese momento.

	// Crea el estado vacío dejando en nulo las listas de columnas, descarte y mazo.
	EstadoGolf() : descarte(nullptr), mazo(nullptr), puntuacion(0), rondas(0) {
		for (int i = 0; i < 7; i++)
			columnas[i] = nullptr;
	}

	// Libera las listas de las 7 columnas, la del descarte y la del mazo.
	~EstadoGolf() {
		for (int i = 0; i < 7; i++)
			delete columnas[i];
		delete descarte;
		delete mazo;
	}

	// Impide copiar el estado porque es dueño de listas reservadas con new.
	EstadoGolf(const EstadoGolf&) = delete;
	void operator = (const EstadoGolf&) = delete;
};

#endif
