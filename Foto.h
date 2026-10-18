#ifndef FOTO_H
#define FOTO_H

#include <iostream>
#include <cstddef>
#include "Cadena.h"
#include "Lista.h"

using namespace std;

class Foto {
private:

    Lista<int> likers;
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

    int cantidadLikers() const { return likers.size(); }
    int getLiker(int k) const { return likers[k]; }
    bool leGusta(int usuario) const;
    bool alternarLike(int usuario);
    void agregarLiker(int usuario);

    void mostrar() const;

    size_t memoriaDinamica() const;
};

#endif