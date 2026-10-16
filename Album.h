#ifndef ALBUM_H
#define ALBUM_H

#include <cstddef>
#include "Cadena.h"
#include "Foto.h"
#include "Lista.h"


class Album {
public:

    enum Privacidad { PUBLICO, PRIVADO, AMIGOS };

private:
    Cadena nombre;
    Cadena ruta;
    Privacidad privacidad;
    Lista<Foto> fotos;

public:

    Album();
    Album(const char* nombre, const char* ruta, Privacidad privacidad);

    const char* getNombre() const { return nombre.c_str(); }
    const char* getRuta() const { return ruta.c_str(); }
    Privacidad getPrivacidad() const { return privacidad; }
    int cantidadFotos() const { return fotos.size(); }
    const Foto& getFoto(int i) const { return fotos[i]; }
    Foto& getFoto(int i) { return fotos[i]; }

    void agregarFoto(const Foto& f) { fotos.agregar(f); }

    bool esVisiblePara(bool esDueno, bool esAmigo) const;

    int indiceSiguiente(int actual) const;
    int indicePrevio(int actual) const;

    static Privacidad privacidadDesdeTexto(const char* texto);
    const char* privacidadATexto() const;

    void mostrar() const;
    size_t memoriaDinamica() const;
};

#endif