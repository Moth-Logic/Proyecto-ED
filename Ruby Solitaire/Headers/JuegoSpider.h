#ifndef JUEGOSPIDER_H
#define JUEGOSPIDER_H

#include <string>
#include <cctype>
#include "Pila.h"
#include "Carta.h"
#include "Lista.h"
#include "Mazo.h"
#include "PilaCartaSpider.h"
#include "EstadoSpider.h"

using std::string;
using std::to_string;

// Controlador del Solitario Spider: reparte 104 cartas en 10 columnas, valida los
// movimientos en escalera, permite repartir y maneja el deshacer y el rehacer.
// mover() y repartir() usan columnas 1..10, mientras que getColumna(i) usa 0..9.
class JuegoSpider {
private:
	PilaCartaSpider columnas[10];
	Mazo mazo;
	int escalerasCompletadas;
	Pila<EstadoSpider*> historialDeshacer;
	Pila<EstadoSpider*> historialRehacer;
	bool partidaTerminada;
	string ultimoError;

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

	// Copia el estado actual de columnas, mazo y contadores en un estado nuevo.
	EstadoSpider* crearSnapshot() {
		EstadoSpider* s = new EstadoSpider();
		for (int i = 0; i < 10; i++) {
			s->columnas[i] = new Lista<Carta>();
			for (int k = 0; k < columnas[i].tamano(); k++)
				s->columnas[i]->append(columnas[i][k]);
		}
		s->mazo = new Lista<Carta>();
		while (mazo.tamano() > 0)
			s->mazo->append(mazo.sacarCarta());
		for (int i = s->mazo->getSize() - 1; i >= 0; i--)
			mazo.agregarCarta(s->mazo->at(i));
		s->reparticionesRestantes = mazo.cantidadReparticiones();
		s->escalerasCompletadas = escalerasCompletadas;
		return s;
	}

	// Restaura un estado guardado en las columnas y el mazo, y libera su memoria.
	void aplicarEstado(EstadoSpider* s) {
		for (int i = 0; i < 10; i++) {
			columnas[i].limpiar();
			for (int k = 0; k < s->columnas[i]->getSize(); k++)
				columnas[i].push(s->columnas[i]->at(k));
		}
		mazo.vaciar();
		for (int i = s->mazo->getSize() - 1; i >= 0; i--)
			mazo.agregarCarta(s->mazo->at(i));
		mazo.establecerReparticiones(s->reparticionesRestantes);
		escalerasCompletadas = s->escalerasCompletadas;
		delete s;
	}

	// Revisa si el grupo forma escalera y si la columna destino acepta la carta base.
	bool movimientoValido(int origen, int destino, int cantidad) {
		if (partidaTerminada) {
			ultimoError = "La partida está terminada.";
			return false;
		}
		else if (origen == destino) {
			ultimoError = "La columna de origen y destino no puede ser la misma.";
			return false;
		}
		PilaCartaSpider& colA = columnas[origen - 1];
		if (!colA.formaEscaleraAscendente(colA.tamano() - cantidad)) {
			ultimoError = "El grupo no forma una escalera valida.";
			return false;
		}
		PilaCartaSpider& colB = columnas[destino - 1];
		int valorBase = colA[colA.tamano() - cantidad].getValor();
		if (!colB.vacia() && colB.tope().getValor() != valorBase + 1) {
			ultimoError = "La columna destino no acepta el grupo.";
			return false;
		}
		return true;
	}

public:
	// Crea la partida sin escaleras completadas y sin terminar.
	JuegoSpider() : escalerasCompletadas(0), partidaTerminada(false) {}
	// Libera los estados guardados al destruir la partida.
	~JuegoSpider() {
		limpiarHistoriales();
	}

	// Reinicia el mazo, las columnas y los contadores para empezar una partida nueva.
	void iniciarPartida() {
		limpiarHistoriales();
		mazo.reiniciar();
		escalerasCompletadas = 0;
		partidaTerminada = false;
		ultimoError = "";
		for (int i = 0; i < 10; i++)
			columnas[i].limpiar();
		for (int i = 0; i < 4; i++)
			for (int k = 0; k < 6; k++)
				columnas[i].push(mazo.sacarCarta());
		for (int i = 4; i < 10; i++)
			for (int k = 0; k < 5; k++)
				columnas[i].push(mazo.sacarCarta());
		for (int i = 0; i < 10; i++)
			columnas[i].voltearTope();
	}

	// Guarda el estado, mueve el grupo de cartas entre columnas y elimina escaleras completas.
	bool mover(int origen, int destino, int cantidad) {
		if (!movimientoValido(origen, destino, cantidad))
			return false;
		historialDeshacer.push(crearSnapshot());
		limpiarRehacer();
		PilaCartaSpider& colA = columnas[origen - 1];
		PilaCartaSpider& colB = columnas[destino - 1];
		Lista<Carta>* grupo = colA.extraerGrupo(cantidad);
		for (int i = 0; i < cantidad; i++)
			colB.push(grupo->at(i));
		delete grupo;
		colA.voltearTope();
		detectarYeliminarEscaleras();
		return true;
	}

	// Reparte una carta boca arriba en cada columna cuando hay repartos disponibles.
	bool repartir() {
		if (partidaTerminada) {
			ultimoError = "La partida esta terminada.";
			return false;
		}
		if (mazo.cantidadReparticiones() < 1 || mazo.tamano() < 10) {
			ultimoError = "No hay reparticiones disponibles.";
			return false;
		}
		for (int i = 0; i < 10; i++)
			if (columnas[i].vacia()) {
				ultimoError = "Ninguna columna puede estar vacia para repartir.";
				return false;
			}
		historialDeshacer.push(crearSnapshot());
		limpiarRehacer();
		for (int i = 0; i < 10; i++) {
			columnas[i].push(mazo.sacarCarta());
			columnas[i].voltearTope();
		}
		mazo.decrementarReparticiones();
		detectarYeliminarEscaleras();
		return true;
	}

	// Devuelve la jugada anterior y la deja disponible para rehacer.
	bool deshacer() {
		if (partidaTerminada || historialDeshacer.isEmpty())
			return false;
		historialRehacer.push(crearSnapshot());
		aplicarEstado(historialDeshacer.pop());
		return true;
	}

	// Reaplica la última jugada que se había deshecho.
	bool rehacer() {
		if (partidaTerminada || historialRehacer.isEmpty())
			return false;
		historialDeshacer.push(crearSnapshot());
		aplicarEstado(historialRehacer.pop());
		return true;
	}

	// Marca la partida como terminada cuando el jugador abandona.
	void abandonar() { partidaTerminada = true; }

	// Elimina de las columnas todas las escaleras K->A que se hayan completado.
	void detectarYeliminarEscaleras() {
		for (int i = 0; i < 10; i++)
			while (columnas[i].esEscaleraCompleta()) {
				columnas[i].extraerEscaleraCompleta();
				columnas[i].voltearTope();
				escalerasCompletadas++;
			}
	}

	// Indica si ya se completaron las 8 escaleras que dan la victoria.
	bool juegoGanado() { return escalerasCompletadas >= 8; }

	// Arma el texto del tablero con el conteo del mazo y las 10 columnas alineadas.
	string mostrarEstado() {
		const int anchoColumna = 11;
		const string separacion = "    ";
		string texto = "Mazo: " + to_string(mazo.tamano()) + " cartas, repartos: "
			+ to_string(mazo.cantidadReparticiones()) + ", escaleras: "
			+ to_string(escalerasCompletadas) + "/8\n\n";
		for (int i = 1; i <= 10; i++) {
			string encabezado = to_string(i);
			while (encabezado.size() < anchoColumna)
				encabezado += " ";
			texto += encabezado + separacion;
		}
		string filas[10];
		int altura = 0;

		for (int i = 0; i < 10; i++) {
			filas[i] = columnas[i].mostrar();

			int lineas = filas[i].empty() ? 0 : 1;

			for (char c : filas[i]) {
				if (c == '\n') {
					lineas++;
				}
			}

			if (lineas > altura) {
				altura = lineas;
			}
		}

		for (int l = 0; l < altura; l++) {
			texto += "\n";

			for (int i = 0; i < 10; i++) {
				string linea = extraerLinea(filas[i], l);

				while (linea.size() < anchoColumna)
					linea += " ";

				texto += linea + separacion;
			}
		}

		return texto;
	}

	// Revisa que la columna de origen esté entre 1 y 10.
	bool origenValido(int origen) {
		if (origen < 1 || origen > 10) {
			ultimoError = "Columna de origen inválida.";
			return false;
		}
		return true;
	}

	// Revisa que la columna de destino esté entre 1 y 10.
	bool destinoValido(int origen, int destino) {
		if (destino < 1 || destino > 10) {
			ultimoError = "Columna de destino inválida.";
			return false;
		}
		return true;
	}

	// Revisa que la cantidad de cartas sea al menos 1 y no supere el tamaño de la columna.
	bool cantidadValida(int origen, int cantidad) {
		if (cantidad < 1 || cantidad > columnas[origen - 1].tamano()) {
			ultimoError = "Cantidad inválida.";
			return false;
		}
		return true;
	}

	// Devuelve la cantidad de escaleras completadas.
	int getEscalerasCompletadas() { return escalerasCompletadas; }
	// Devuelve los repartos que le quedan al mazo.
	int getReparticionesRestantes() { return mazo.cantidadReparticiones(); }
	// Devuelve la cantidad de cartas que quedan en el mazo.
	int getCartasMazo() { return mazo.tamano(); }
	// Indica si la partida ya terminó.
	bool estaPartidaTerminada() { return partidaTerminada; }
	// Devuelve la columna indicada con índices de 0 a 9.
	PilaCartaSpider& getColumna(int indice) { return columnas[indice]; }
	// Devuelve el último mensaje de error generado por una validación.
	string getUltimoError() { return ultimoError; }

	// Devuelve la línea 'linea' (0-based) de un texto separado por saltos de línea.
	static string extraerLinea(const string& texto, int linea) {
		size_t pos = 0;
		for (int i = 0; i < linea; i++) {
			pos = texto.find('\n', pos);
			if (pos == string::npos)
				return "";
			pos++;
		}
		size_t fin = texto.find('\n', pos);
		return texto.substr(pos, fin == string::npos ? string::npos : fin - pos);
	}
};

#endif
