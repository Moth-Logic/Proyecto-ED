#ifndef JUEGOGOLF_H
#define JUEGOGOLF_H

#include <cstdlib>
#include <string>
#include "Carta.h"
#include "EstadoGolf.h"
#include "Lista.h"
#include "MazoGolf.h"
#include "Pila.h"
#include "PilaCartaGolf.h"

using std::string;
using std::to_string;

// Controlador del Solitario Golf. Se reparten 7 columnas de 5 cartas y el tope de
// la primera columna inicia el descarte. En cada jugada se pasa el tope de una
// columna al descarte cuando su valor es contiguo al de la carta superior del
// descarte, con el As y la K también contiguos. Cuando el jugador se queda sin
// jugadas puede repartir una carta nueva a cada columna con lo que queda en el
// mazo. La partida se gana al dejar vacías las 7 columnas.
// mover() usa columnas 1..7, mientras que getColumna(i) usa 0..6.
class JuegoGolf {
private:
	static const int CANTIDAD_COLUMNAS = 7;
	static const int CARTAS_POR_COLUMNA = 5;

	PilaCartaGolf columnas[CANTIDAD_COLUMNAS];
	MazoGolf mazoGolf;
	Pila<Carta> descarte;
	Pila<EstadoGolf*> historialDeshacer;
	Pila<EstadoGolf*> historialRehacer;
	int puntuacion;
	int rondas;
	bool partidaTerminada;
	string ultimoError;

	// Copia el estado actual de columnas, descarte, mazo y contadores en un estado nuevo.
	EstadoGolf* crearSnapshot() {
		EstadoGolf* estado = new EstadoGolf();
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++) {
			estado->columnas[i] = new Lista<Carta>();
			for (int k = 0; k < columnas[i].tamano(); k++)
				estado->columnas[i]->append(columnas[i][k]);
		}
		estado->descarte = new Lista<Carta>();
		Pila<Carta> aux;
		while (!descarte.isEmpty())
			aux.push(descarte.pop());
		while (!aux.isEmpty()) {
			Carta carta = aux.pop();
			descarte.push(carta);
			estado->descarte->append(carta);
		}
		estado->mazo = mazoGolf.copiarCartas();
		estado->puntuacion = puntuacion;
		estado->rondas = rondas;
		return estado;
	}

	// Restaura un estado guardado en las columnas, el descarte y el mazo, y libera su memoria.
	void aplicarEstado(EstadoGolf* estado) {
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++) {
			columnas[i].limpiar();
			for (int k = 0; k < estado->columnas[i]->getSize(); k++)
				columnas[i].push(estado->columnas[i]->at(k));
		}
		descarte.clear();
		for (int k = 0; k < estado->descarte->getSize(); k++)
			descarte.push(estado->descarte->at(k));
		mazoGolf.restaurarCartas(estado->mazo);
		puntuacion = estado->puntuacion;
		rondas = estado->rondas;
		delete estado;
	}

	// Libera los estados guardados para rehacer.
	void limpiarRehacer() {
		while (!historialRehacer.isEmpty())
			delete historialRehacer.pop();
	}

	// Libera los estados guardados para deshacer y para rehacer.
	void limpiarHistoriales() {
		while (!historialDeshacer.isEmpty())
			delete historialDeshacer.pop();
		limpiarRehacer();
	}

	// Revisa que la columna exista, tenga cartas y que su tope sea contiguo al descarte.
	bool movimientoValido(int origen) {
		if (partidaTerminada) {
			ultimoError = "La partida está terminada.";
			return false;
		}
		if (origen < 1 || origen > CANTIDAD_COLUMNAS) {
			ultimoError = "Columna de origen inválida.";
			return false;
		}
		PilaCartaGolf& columna = columnas[origen - 1];
		if (columna.vacia()) {
			ultimoError = "La columna de origen está vacía.";
			return false;
		}
		if (descarte.isEmpty()) {
			ultimoError = "El descarte está vacío.";
			return false;
		}

		int valorOrigen = columna.tope().getValor();
		int valorDescarte = descarte.top().getValor();

		bool esContiguo = std::abs(valorOrigen - valorDescarte) == 1;
		bool esCicloKA = (valorOrigen == 1 && valorDescarte == 13) || (valorOrigen == 13 && valorDescarte == 1);

		if (!esContiguo && !esCicloKA) {
			ultimoError = "La carta no es contigua a la carta del descarte.";
			return false;
		}
		return true;
	}

public:
	// Crea la partida sin puntos, sin rondas y sin terminar.
	JuegoGolf() : puntuacion(0), rondas(0), partidaTerminada(false) {}
	// Libera los estados guardados al destruir la partida.
	~JuegoGolf() { limpiarHistoriales(); }

	// Reinicia el mazo y las columnas, reparte 5 cartas por columna y arranca el descarte.
	void iniciarPartida() {
		limpiarHistoriales();
		mazoGolf.reiniciar();
		puntuacion = 0;
		rondas = 1;
		partidaTerminada = false;
		ultimoError = "";
		descarte.clear();
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++)
			columnas[i].limpiar();
		for (int k = 0; k < CARTAS_POR_COLUMNA; k++)
			for (int i = 0; i < CANTIDAD_COLUMNAS; i++) {
				columnas[i].push(mazoGolf.sacarCarta());
				columnas[i].voltearTope();
			}
		// El tope de la primera columna es la primera carta del descarte.
		descarte.push(columnas[0].pop());
	}

	// Pasa el tope de la columna indicada al descarte si la jugada es válida.
	bool mover(int origen) {
		if (!movimientoValido(origen))
			return false;
		historialDeshacer.push(crearSnapshot());
		limpiarRehacer();
		descarte.push(columnas[origen - 1].pop());
		puntuacion++;
		if (juegoGanado())
			partidaTerminada = true;
		return true;
	}

	// Reparte una carta boca arriba en cada columna mientras queden cartas suficientes.
	bool repartir() {
		if (partidaTerminada) {
			ultimoError = "La partida está terminada.";
			return false;
		}
		if (mazoGolf.cantidadReparticiones() < 1) {
			ultimoError = "No hay cartas suficientes en el mazo para repartir.";
			return false;
		}
		historialDeshacer.push(crearSnapshot());
		limpiarRehacer();
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++) {
			columnas[i].push(mazoGolf.sacarCarta());
			columnas[i].voltearTope();
		}
		rondas++;
		return true;
	}

	// Devuelve la jugada anterior y la deja disponible para rehacer.
	bool deshacer() {
		if (partidaTerminada) {
			ultimoError = "La partida está terminada.";
			return false;
		}
		if (historialDeshacer.isEmpty()) {
			ultimoError = "No hay movimientos para deshacer.";
			return false;
		}
		historialRehacer.push(crearSnapshot());
		aplicarEstado(historialDeshacer.pop());
		return true;
	}

	// Reaplica la última jugada que se había deshecho.
	bool rehacer() {
		if (partidaTerminada) {
			ultimoError = "La partida está terminada.";
			return false;
		}
		if (historialRehacer.isEmpty()) {
			ultimoError = "No hay movimientos para rehacer.";
			return false;
		}
		historialDeshacer.push(crearSnapshot());
		aplicarEstado(historialRehacer.pop());
		return true;
	}

	// Marca la partida como terminada cuando el jugador abandona.
	void abandonar() { partidaTerminada = true; }

	// Indica si ya se vaciaron las 7 columnas, que es la condición de victoria.
	bool juegoGanado() {
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++)
			if (!columnas[i].vacia())
				return false;
		return true;
	}

	// Indica si la partida ya terminó.
	bool estaPartidaTerminada() { return partidaTerminada; }

	// Arma el texto del tablero con el conteo del mazo y una línea por columna.
	string mostrarEstado() {
		string texto = "Mazo: " + to_string(mazoGolf.tamano()) + " cartas, repartos: "
			+ to_string(mazoGolf.cantidadReparticiones()) + ", ronda: " + to_string(rondas)
			+ ", puntos: " + to_string(puntuacion) + "\n";
		texto += "Descarte: " + (descarte.isEmpty() ? string("-") : descarte.top().toString()) + "\n\n";
		for (int i = 0; i < CANTIDAD_COLUMNAS; i++)
			texto += to_string(i + 1) + ". " + columnas[i].mostrar() + "\n";
		return texto;
	}

	// Devuelve la columna indicada con índices de 0 a 6.
	PilaCartaGolf& getColumna(int indice) { return columnas[indice]; }
	// Devuelve la carta del tope del descarte o una carta vacía si no hay ninguna.
	Carta getDescarteTope() { return descarte.isEmpty() ? Carta() : descarte.top(); }
	// Devuelve cuántas cartas se pasaron al descarte.
	int getPuntuacion() { return puntuacion; }
	// Devuelve cuántos repartos se dieron hasta el momento.
	int getRondas() { return rondas; }
	// Devuelve la cantidad de cartas que quedan en el mazo.
	int getCartasMazo() { return mazoGolf.tamano(); }
	// Devuelve los repartos completos que todavía se pueden dar.
	int getReparticionesRestantes() { return mazoGolf.cantidadReparticiones(); }
	// Devuelve la cantidad de cartas del descarte.
	int getCartasDescarte() { return descarte.getSize(); }
	// Devuelve el último mensaje de error generado por una validación.
	string getUltimoError() { return ultimoError; }
};

#endif
