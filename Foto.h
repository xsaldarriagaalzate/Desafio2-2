#ifndef FOTO_H
#define FOTO_H

#include <iostream>
#include <cstddef>
#include "Cadena.h"

using namespace std;

class Foto {
private:
    Cadena fechaCaptura;
    long tamano;
    Cadena rutaArchivo;
    int likes;

public:

    Foto();
    Foto(const char* fechaCaptura, long tamano, const char* rutaArchivo, int likes);

    const char* getFechaCaptura() const { return fechaCaptura.cadena_char(); }
    long getTamano() const { return tamano; }
    const char* getRutaArchivo() const { return rutaArchivo.cadena_char(); }
    int getLikes() const { return likes; }

    void darLike() { likes++; }
    void quitarLike() { if (likes > 0) likes--; }

    void mostrar() const;
};

#endif