#ifndef PANTALLASPIDER_H
#define PANTALLASPIDER_H

#include <string>
#include <vector>
#include "JuegoSpider.h"
#include "Cursor.h"
#include "Consola.h"
#include "Lienzo.h"
#include "Render.h"
#include "Victoria.h"

// Ciclo de la partida de Spider con controles de teclado.
// Vuelve al menú cuando el jugador abandona o cuando termina la victoria y no quiere otra partida.
inline void jugarSpider() {
	bool otraPartida = true;
	while (otraPartida) {
		otraPartida = false;

		JuegoSpider juego;
		juego.iniciarPartida();
		CursorSpider cursor;
		Efectos ef;
		bool salir = false;

		vaciarTeclado();
		limpiarPantalla();
		ocultarCursor();

		// Avisa cuando una escalera K->A se completa y sale del tablero.
		auto avisarEscalera = [&](int antes) {
			if (juego.getEscalerasCompletadas() > antes)
				ef.avisar("Sequence removed! " + std::to_string(juego.getEscalerasCompletadas()) + "/8", Color::Verde, 1800);
		};

		// Solo se lanza la victoria cuando el juego confirma que se completaron las 8 escaleras.
		auto revisarVictoria = [&]() {
			if (juego.juegoGanado()) {
				std::string resumen = "Sequences 8/8   ·   Deals left " + std::to_string(juego.getReparticionesRestantes());
				otraPartida = reproducirVictoria(resumen);
				salir = true;
			}
		};

		while (!salir) {
			dibujar(renderSpider(juego, cursor, ef).render());
			esperar(16);
			ef.tick(16);

			for (Entrada e = leerEntrada(); e.tecla != Tecla::Ninguna && !salir; e = leerEntrada()) {
				switch (e.tecla) {
				case Tecla::Izquierda:
					cursor.izquierda();
					break;

				case Tecla::Derecha:
					cursor.derecha();
					break;

				case Tecla::Espacio:
					if (cursor.estaAgarrando()) {
						cursor.cancelar();
						ef.avisar("Selection cancelled.", Color::Amarillo);
					}
					else if (cursor.agarrar(juego)) {
						ef.avisar("Holding " + std::to_string(cursor.getCantidad()) + " card(s). Pick a column and press ENTER.", Color::Cian);
					}
					else {
						ef.avisar("No movable run on top of this column.", Color::Amarillo);
					}
					break;

				case Tecla::Escape:
					if (cursor.estaAgarrando()) {
						cursor.cancelar();
						ef.avisar("Selection cancelled.", Color::Amarillo);
					}
					break;

				case Tecla::Enter: {
					if (!cursor.estaAgarrando()) {
						ef.avisar("Press SPACE on a column to grab cards first.", Color::Amarillo);
						break;
					}
					int origen = cursor.getOrigen();
					int destino = cursor.getColumna();
					int total = juego.getColumna(origen).tamano();
					std::vector<std::string> lineasMovidas = lineasDeRango(juego, origen, total - cursor.getCantidad(), total);

					int escalerasAntes = juego.getEscalerasCompletadas();
					if (cursor.soltar(juego)) {
						ef.volar(lineasMovidas, colX(origen), colX(destino), 220);
						ef.pulsar(destino, 260);
						ef.avisar("Moved.", Color::Verde, 1200);
						avisarEscalera(escalerasAntes);
						revisarVictoria();
					}
					else {
						// El juego no cambió nada. La selección sigue activa para elegir otro destino.
						ef.avisar("INVALID MOVE — " + juego.getUltimoError(), Color::Rojo);
						ef.msError = 400;
					}
					break;
				}

				case Tecla::Letra:
					if (e.letra == 'R') {
						cursor.cancelar();
						int escalerasAntes = juego.getEscalerasCompletadas();
						if (juego.repartir()) {
							ef.avisar("New row dealt.", Color::Verde);
							avisarEscalera(escalerasAntes);
							revisarVictoria();
						}
						else {
							ef.avisar("DEAL FAILED — " + juego.getUltimoError(), Color::Rojo);
						}
					}
					else if (e.letra == 'U') {
						cursor.cancelar();
						if (juego.deshacer()) ef.avisar("Undone.", Color::Amarillo);
						else ef.avisar("Nothing to undo.", Color::Gris);
					}
					else if (e.letra == 'Y') {
						cursor.cancelar();
						if (juego.rehacer()) ef.avisar("Redone.", Color::Amarillo);
						else ef.avisar("Nothing to redo.", Color::Gris);
					}
					else if (e.letra == 'Q') {
						juego.abandonar();
						salir = true;
					}
					break;

				default:
					break;
				}
			}
		}

		limpiarPantalla();
		mostrarCursor();
		vaciarTeclado();
	}
}

#endif
