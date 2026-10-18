#include "RedAmistades.h"

RedAmistades::RedAmistades() : n(0) {}

RedAmistades::RedAmistades(int n) : n(n) {
    bits = new unsigned char[total];
    for (long k = 0; k < total; k++) bits[k] = 0;
}

RedAmistades::RedAmistades(const RedAmistades& otra) : n(otra.n) {
    long total = bytesNecesarios(n);
    bits = new unsigned char[total];
    for (long k = 0; k < total; k++) bits[k] = otra.bits[k];
}

RedAmistades& RedAmistades::operator=(const RedAmistades& otra) {
    if (this == &otra) return *this;
    delete[] bits;
    n = otra.n;
    long total = bytesNecesarios(n);
    bits = new unsigned char[total];
    for (long k = 0; k < total; k++) bits[k] = otra.bits[k];
    return *this;
}

RedAmistades::~RedAmistades() { delete[] bits; }

void RedAmistades::poner(int i, int j) {
    long k = posicion(i, j);
    bits[k / 8] |= (1 << (k % 8));
}

void RedAmistades::conectar(int a, int b) {
    if (a == b) return;
    poner(a, b);
    poner(b, a);
}

bool RedAmistades::sonAmigos(int a, int b) const {
    long k = posicion(a, b);
    return (bits[k / 8] >> (k % 8)) & 1;
}

int RedAmistades::amigosEnComun(int a, int b) const {
    int c = 0;
    for (int k = 0; k < n; k++)
        if (sonAmigos(a, k) && sonAmigos(b, k)) c++;
    return c;
}

size_t RedAmistades::memoria() const {
    return sizeof(*this) + bytesNecesarios(n);
}