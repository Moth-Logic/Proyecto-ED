#ifndef MAZO_H
#define MAZO_H

#include <cstdlib>
#include <ctime>
#include "Pila.h"
#include "Carta.h"

// Mazo del Spider: 104 cartas (2 naipes de espadas). El tope de la pila es
// la próxima carta a repartir. Máximo 5 reparticiones de 10 cartas.
class Mazo {
private:
	Pila<Carta> cartas;
	int reparticionesRestantes;

	void llenar() {
		for (int valor = 1; valor <= 13; valor++)
			for (int i = 0; i < 8; i++)
				cartas.push(Carta(valor, 'E'));
	}

public:
	Mazo() : reparticionesRestantes(5) {
		std::srand((unsigned)std::time(nullptr));
		reiniciar();
	}

	void reiniciar() {
		cartas.clear();
		llenar();
		barajar();
		reparticionesRestantes = 5;
	}

	// Fisher-Yates sobre un arreglo temporal (104 es el tamaño fijo del mazo).
	void barajar() {
		Carta arr[104];
		int n = cartas.getSize();
		for (int i = 0; i < n; i++)
			arr[i] = cartas.pop();
		for (int i = n - 1; i > 0; i--) {
			int j = std::rand() % (i + 1);
			Carta temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
		}
		for (int i = 0; i < n; i++)
			cartas.push(arr[i]);
	}

	Carta sacarCarta() { return cartas.pop(); }
	int tamano() { return cartas.getSize(); }
	int cantidadReparticiones() { return reparticionesRestantes; }
	void decrementarReparticiones() { reparticionesRestantes--; }

	// Apoyo para snapshots (deshacer/rehacer): vaciar, recargar y restaurar.
	void vaciar() { cartas.clear(); }
	void agregarCarta(const Carta& carta) { cartas.push(carta); }
	void establecerReparticiones(int cantidad) { reparticionesRestantes = cantidad; }
};

#endif
