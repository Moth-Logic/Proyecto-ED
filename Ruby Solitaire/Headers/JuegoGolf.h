#ifndef JUEGOGOLF_H
#define JUEGOGOLF_H

#include <string>
#include <cctype>
#include "Pila.h"
#include "Carta.h"
#include "Lista.h"
#include "MazoGolf.h"
#include "PilaCartaGolf.h"
#include "EstadoGolf.h"

using std::string;
using std::to_string;

class JuegoGolf {
private: 
	PilaCartaGolf columnas[7];
	MazoGolf mazoGolf;
	int puntuación;
	Pila<EstadoGolf*> historialDeshacer;
	Pila <EstadoGolf*> historialRehacer;
	Pila <Carta> descarte;
public:
	JuegoGolf();
	~JuegoGolf();
	EstadoGolf* crearSnapshot();
	void aplicarEstado(EstadoGolf* estado);
	bool mover(int origen, int destino);
	bool repartir();
	bool deshacer();
	bool rehacer();
	PilaCartaGolf& getColumna(int indice);
	int getPuntuación() const;
	int getReparticionesRestantes() const;
	int getCartasDescarte() const;
	string mostrarMesa() const;
	string getUltimoError() const;
};

#endif
