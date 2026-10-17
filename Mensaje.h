#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include "Cadena.h"
#include "Fecha.h"

class Mensaje {
public:
    static const int MAX_TEXTO = 512;

private:
    int emisor;
    int receptor;
    Cadena texto;
    Fecha fecha;
    bool leido;

public:

    Mensaje();
    Mensaje(int e, int r, const char* t, const Fecha& f, bool l = false);
    Mensaje(int e, int r, const char* t);

    static bool textoValido(const char* t);

    int getEmisor() const;
    int getReceptor() const;
    const char* getTexto() const;
    const Fecha& getFecha() const;
    bool estaLeido() const;

    void marcarLeido();
    void setLeido(bool valor);

    unsigned long memoria() const;

    void mostrar() const;
    void mostrarCon(const char* nombreEmisor, const char* nombreReceptor) const;

    bool operator<(const Mensaje& o) const;    // por fecha (para ordenar)
    bool operator==(const Mensaje& o) const;

    friend std::ostream& operator<<(std::ostream& os, const Mensaje& m);
};

#endif