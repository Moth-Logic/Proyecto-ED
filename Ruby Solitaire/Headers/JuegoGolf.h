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
}


#endif
