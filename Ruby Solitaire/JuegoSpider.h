#ifndef JUEGOSPIDER_H
#define JUEGOSPIDER_H

#include <string>
#include "Pila.h"
#include "Carta.h"
#include "Lista.h"
#include "Mazo.h"
#include "PilaCartaSpider.h"
#include "EstadoSpider.h"

using std::string;
using std::to_string;

// Controlador del Solitario Spider.
// mover() y repartir() usan columnas 1..10; getColumna(i) usa 0..9.
// Reglas: 104 cartas en 10 columnas (4 de 6 y 6 de 5, solo el tope boca
// arriba); se mueven grupos boca arriba en escalera a una columna cuya carta
// superior valga 1 más que la base del grupo (o a una vacía); repartir pone
// una carta boca arriba en cada columna si ninguna está vacía (5 repartos);
// cada escalera completa K->A se elimina; se gana con 8.
class JuegoSpider {
private:
	PilaCartaSpider columnas[10];
	Mazo mazo;
	int escalerasCompletadas;
	Pila<EstadoSpider*> historialDeshacer;
	Pila<EstadoSpider*> historialRehacer;
	bool partidaTerminada;
	string ultimoError;

	void limpiarRehacer() {
		while (!historialRehacer.isEmpty())
			delete historialRehacer.pop();
	}

	void limpiarHistoriales() {
		while (!historialDeshacer.isEmpty())
			delete historialDeshacer.pop();
		limpiarRehacer();
	}

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
		// Recarga el mazo sin alterarlo (la lista quedó en orden de salida).
		for (int i = s->mazo->getSize() - 1; i >= 0; i--)
			mazo.agregarCarta(s->mazo->at(i));
		s->reparticionesRestantes = mazo.cantidadReparticiones();
		s->escalerasCompletadas = escalerasCompletadas;
		return s;
	}

	// Aplica el snapshot y libera su memoria.
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
	JuegoSpider() : escalerasCompletadas(0), partidaTerminada(false) {}
	~JuegoSpider() {
		limpiarHistoriales();
	}

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

	bool deshacer() {
		if (partidaTerminada || historialDeshacer.isEmpty())
			return false;
		historialRehacer.push(crearSnapshot());
		aplicarEstado(historialDeshacer.pop());
		return true;
	}

	bool rehacer() {
		if (partidaTerminada || historialRehacer.isEmpty())
			return false;
		historialDeshacer.push(crearSnapshot());
		aplicarEstado(historialRehacer.pop());
		return true;
	}

	void abandonar() { partidaTerminada = true; }

	void detectarYeliminarEscaleras() {
		for (int i = 0; i < 10; i++)
			while (columnas[i].esEscaleraCompleta()) {
				columnas[i].extraerEscaleraCompleta();
				columnas[i].voltearTope();
				escalerasCompletadas++;
			}
	}

	bool juegoGanado() { return escalerasCompletadas >= 8; }

	// Tablero como texto. Cada columna usa 6 lienas para las cartas tapadas y 7 para las que están volteadas. 5 espacios de separación.
	string mostrarEstado() {
		const int anchoColumna = 9;
		const string separacion = "     ";
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

	bool origenValido(int origen) {
		if (origen < 1 || origen > 10) {
			ultimoError = "Columna de origen inválida.";
			return false;
		}
		return true;
	}

	bool destinoValido(int origen, int destino) {
		if (destino < 1 || destino > 10) {
			ultimoError = "Columna de destino inválida.";
			return false;
		}
		return true;
	}

	bool cantidadValida(int origen, int cantidad) {
		if (cantidad < 1 || cantidad > columnas[origen - 1].tamano()) {
			ultimoError = "Cantidad inválida.";
			return false;
		}
		return true;
	}

	int getEscalerasCompletadas() { return escalerasCompletadas; }
	int getReparticionesRestantes() { return mazo.cantidadReparticiones(); }
	int getCartasMazo() { return mazo.tamano(); }
	bool estaPartidaTerminada() { return partidaTerminada; }
	PilaCartaSpider& getColumna(int indice) { return columnas[indice]; }
	string getUltimoError() { return ultimoError; }

	// Línea 'n' (0-based) de un texto con '\n'; espacios si no existe.
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
