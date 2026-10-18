#ifndef CADENA_H
#define CADENA_H

#include <cstddef>

class Cadena {
private:
    char* datos;
    int len;

public:

    Cadena();
    Cadena(const char* texto);

    Cadena(const char* s, int n);

    Cadena(const Cadena& otraCadena);
    Cadena& operator=(const Cadena& otraCadena);

    ~Cadena();

    bool operator==(const Cadena& otraCadena) const;
    bool operator!=(const Cadena& otraCadena) const;
    bool operator<(const Cadena& otraCadena) const;

    const char* cadena_char() const { return datos; }
    int largo() const { return len; }

};

#endif // CADENA_H