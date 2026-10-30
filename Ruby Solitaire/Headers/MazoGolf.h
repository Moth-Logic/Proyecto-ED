#ifndef MAZOGOLF_H
#define MAZOGOLF_H

#include <cstdlib>
#include <ctime>
#include <string>
#include "Carta.h"
#include "Lista.h"
#include "Pila.h"

using std::string;
using std::srand;
using std::time;

// Mazo con las 52 cartas de los cuatro palos, sin copias. Se reparten 7 columnas
// de 5 cartas y las 17 sobrantes quedan en el mazo para los repartos posteriores.
class MazoGolf {
private:
	Pila<Carta> cartas;

	// Llena el mazo con las 52 cartas antes de barajarlo.
	void llenar() {
		const string palos = "EDTC";
		for (char paloActual : palos)
			for (int valor = 1; valor <= 13; valor++)
				cartas.push(Carta(valor, paloActual));
	}

public:
	// Cartas que se necesitan para dar un reparto completo en las 7 columnas.
	static const int CARTAS_POR_REPARTO = 7;

	// Prepara el generador aleatorio y deja un mazo barajado listo para jugar.
	MazoGolf() {
		srand((unsigned)time(nullptr));
		reiniciar();
	}

	// Vuelve a llenar el mazo y lo baraja.
	void reiniciar() {
		cartas.clear();
		llenar();
		barajar();
	}

	// Baraja las 52 cartas con el algoritmo Fisher-Yates sobre un arreglo temporal.
	void barajar() {
		Carta arr[52];
		int n = cartas.getSize();
		for (int i = 0; i < n; i++)
			arr[i] = cartas.pop();
		for (int i = n - 1; i > 0; i--) {
			int j = rand() % (i + 1);
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
	// Devuelve cuántos repartos completos se pueden dar todavía con las cartas que quedan.
	int cantidadReparticiones() { return cartas.getSize(); }

	// Vacía el mazo para reconstruirlo desde un estado guardado.
	void vaciar() { cartas.clear(); }
	// Apila una carta en el tope del mazo al reconstruirlo.
	void agregarCarta(const Carta& carta) { cartas.push(carta); }

	// Devuelve una copia de las cartas en orden fondo->tope, así la última posición
	// es la próxima carta a repartir. El llamador debe liberar la lista.
	Lista<Carta>* copiarCartas() {
		Lista<Carta>* copia = new Lista<Carta>();
		Pila<Carta> aux;
		while (!cartas.isEmpty())
			aux.push(cartas.pop());
		while (!aux.isEmpty()) {
			Carta carta = aux.pop();
			cartas.push(carta);
			copia->append(carta);
		}
		return copia;
	}

	// Reconstruye el mazo desde una copia hecha con copiarCartas().
	void restaurarCartas(Lista<Carta>* copia) {
		cartas.clear();
		for (int i = 0; i < copia->getSize(); i++)
			cartas.push(copia->at(i));
	}
};

#endif
