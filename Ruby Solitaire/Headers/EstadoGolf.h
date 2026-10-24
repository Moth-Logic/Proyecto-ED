#ifndef ESTADOGOLF_H
#define ESTADOGOLF_H

#include "PilaCartaGolf.h"
#include "Pila.h"
#include "Carta.h"

class EstadoGolf {
private:
    PilaCartaGolf columnas[7];
    Pila<Carta> descarte;
    int puntuacion;
    int rondas;

public:
    EstadoGolf() : puntuacion(0), rondas(0) {}

    EstadoGolf(const PilaCartaGolf cols[7], const Pila<Carta>& desc, int punt, int rds) 
        : descarte(desc), puntuacion(punt), rondas(rds) {
        for (int i = 0; i < 7; ++i) {
            columnas[i] = cols[i];
        }
    }

    const PilaCartaGolf& getColumna(int indice) const {
        return columnas[indice];
    }

    const Pila<Carta>& getDescarte() const {
        return descarte;
    }

    int getPuntuacion() const {
        return puntuacion;
    }

    int getRondas() const {
        return rondas;
    }
};

#endif
