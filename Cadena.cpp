#include <iostream>
#include "Cadena.h"

using namespace std;


static int longitud(const char* s) {
    int n = 0;
    while (s[n] != '\0') n++;
    return n;
}

static char* duplicar(const char* s, int n) {
    char* r = new char[n + 1];
    for (int i = 0; i <= n; i++) r[i] = s[i];
    return r;
}

Cadena::Cadena() : datos(duplicar("", 0)), len(0) {}

Cadena::Cadena(const char* texto) : datos(nullptr), len(0) {
    if (texto == nullptr) texto = "";
    len = longitud(texto);
    datos = duplicar(texto,len);
}

Cadena::Cadena(const Cadena& otraCadena) {
    datos = duplicar(otraCadena.datos, otraCadena.len);
    len = otraCadena.len;
}

Cadena& Cadena::operator=(const Cadena& otraCadena) {
    if (this == &otraCadena) return *this;
    delete[] datos;
    len = otraCadena.len;
    datos = duplicar(otraCadena.datos, len);
    return *this;
}

Cadena::~Cadena() { delete[] datos; }

bool Cadena::operator==(const Cadena& otraCadena) const {
    if (len != otraCadena.len) return false;
    for (int i = 0; i < len; i++) {
        if (datos[i] != otraCadena.datos[i]) return false;
    }
    return true;
}

bool Cadena::operator!=(const Cadena& otraCadena) const {
    return !(*this == otraCadena);
}

bool Cadena::operator<(const Cadena& otraCadena) const {
    int i = 0;
    while (datos[i] != '\0' && otraCadena.datos[i] != '\0') {
        if (datos[i] != otraCadena.datos[i]) return datos[i] < otraCadena.datos[i];
        i++;
    }
    return datos[i] == '\0' && otraCadena.datos[i] != '\0';
}

const char* Cadena::cadena_cha()  { return datos; }

int Cadena::largo() { return len; }