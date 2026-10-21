#ifndef MAZOGOLF_H
#define MAZOGOLF_H

#include <cstdlib>
#include <ctime>
#include "Pila.h"
#include "Carta.h"

// Mazo de 104 cartas de espadas (8 copias por valor) con 5 repartos de 10 cartas.
class MazoGolf {
private:
	Pila<Carta> cartas;
	int reparticionesRestantes;

	// Llena el mazo con las 104 cartas antes de barajarlo.
	void llenar() {
		for (int valor = 1; valor <= 13; valor++)
			for (int i = 0; i < 8; i++)
				cartas.push(Carta(valor, 'E'));
	}

public:
	// Prepara el generador aleatorio y deja un mazo barajado listo para jugar.
	MazoGolf() : reparticionesRestantes(5) {
		std::srand((unsigned)std::time(nullptr));
		reiniciar();
	}

	// Vuelve a llenar el mazo, lo baraja y restaura los 5 repartos.
	void reiniciar() {
		cartas.clear();
		llenar();
		barajar();
		reparticionesRestantes = 5;
	}

	// Baraja las 104 cartas con el algoritmo Fisher-Yates sobre un arreglo temporal.
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

	// Saca la próxima carta del tope del mazo.
	Carta sacarCarta() { return cartas.pop(); }
	// Devuelve la cantidad de cartas que quedan en el mazo.
	int tamano() { return cartas.getSize(); }
	// Devuelve cuántos repartos le quedan al mazo.
	int cantidadReparticiones() { return reparticionesRestantes; }
	// Descuenta un reparto del contador del mazo.
	void decrementarReparticiones() { reparticionesRestantes--; }

	// Vacía el mazo para reconstruirlo desde un estado guardado.
	void vaciar() { cartas.clear(); }
	// Apila una carta en el tope del mazo al reconstruirlo.
	void agregarCarta(const Carta& carta) { cartas.push(carta); }
	// Fija los repartos restantes al reconstruir un estado guardado.
	void establecerReparticiones(int cantidad) { reparticionesRestantes = cantidad; }
};

#endif
