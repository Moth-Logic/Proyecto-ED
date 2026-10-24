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
	string ultimoError; 
	int rondas;
public:
	JuegoGolf();
	~JuegoGolf();
	EstadoGolf* crearSnapshot();
	void aplicarEstado(EstadoGolf* estado);
    bool movimientoValido(int origen) {
        if (origen < 1 || origen > 7) {
            ultimoError = "Columna de origen inválida.";
            return false;
        }

        int posiciones = origen - 1;

        if (columnas[posiciones].vacia()) {
			ultimoError = "La columna de origen está vacía.";	
            return false;
        }

        if (!descarte.isEmpty()) {
            Carta cartaOrigen = columnas[posiciones].tope();
            Carta cartaDescarte = descarte.top(); 

            int valOrigen = cartaOrigen.getValor();
            int valDescarte = cartaDescarte.getValor();

            bool esContiguo = (std::abs(valOrigen - valDescarte) == 1);

            bool esCicloKA = (valOrigen == 1 && valDescarte == 13) || (valOrigen == 13 && valDescarte == 1);

            if (!esContiguo && !esCicloKA) {
                ultimoError = "La carta no es contigua a la carta del descarte.";
                return false;
            }
        }

        return true;
    }
    bool mover(int origen) {

        if (!movimientoValido(origen)) {
            return false;
        }

        historialDeshacer.push(crearSnapshot());
        limpiarRehacer();

        int idx = origen - 1;
        Carta cartaMovida = columnas[idx].pop();
        descarte.push(cartaMovida);

        return true;
    }
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
