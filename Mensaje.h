#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include "Fecha.h"

class Mensaje {
public:
    static const int MAX_TEXTO = 512;

private:
    char* emisor;
    char* receptor;
    char* texto;
    Fecha fecha;
    bool leido;

    static int longitud(const char* s);
    static char* duplicar(const char* s);

public:
    Mensaje();
    Mensaje(const char* emisor, const char* receptor, const char* texto,
            const Fecha& fecha, bool leido = false);
    Mensaje(const Mensaje& otro);
    Mensaje& operator=(const Mensaje& otro);
    ~Mensaje();

    static bool textoValido(const char* texto);

    const char* getEmisor() const;
    const char* getReceptor() const;
    const char* getTexto() const;
    const Fecha& getFecha() const;
    bool estaLeido() const;

    void marcarLeido();
    void setLeido(bool valor);

    unsigned long memoria() const;

    void mostrar() const;

    bool operator<(const Mensaje& otro) const;
    bool operator==(const Mensaje& otro) const;

    friend std::ostream& operator<<(std::ostream& os, const Mensaje& m);
};

#endif // MENSAJE_H