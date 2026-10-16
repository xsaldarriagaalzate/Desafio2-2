#ifndef FOTO_H
#define FOTO_H

#include <cstddef>
#include "Cadena.h"

class Foto {
private:
    Cadena fechaCaptura;
    long tamano;
    Cadena rutaArchivo;
    int likes;

public:

    Foto();
    Foto(const char* fechaCaptura, long tamano, const char* rutaArchivo, int likes);

    const char* getFechaCaptura() const { return fechaCaptura.c_str(); }
    long getTamano() const { return tamano; }
    const char* getRutaArchivo() const { return rutaArchivo.c_str(); }
    int getLikes() const { return likes; }

    void darLike() { likes++; }
    void quitarLike() { if (likes > 0) likes--; }

    void mostrar() const;

    size_t memoriaDinamica() const;
};

#endif