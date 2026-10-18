#include "MatrizAmistad.h"

MatrizAmistad::MatrizAmistad() : celdas(nullptr), n(0) {}

MatrizAmistad::MatrizAmistad(const MatrizAmistad& otra) : celdas(nullptr), n(otra.n) {
    size_t total = (size_t)n * n;
    if (total > 0) {
        celdas = new char[total];
        for (size_t k = 0; k < total; k++) celdas[k] = otra.celdas[k];
    }
}

MatrizAmistad& MatrizAmistad::operator=(const MatrizAmistad& otra) {
    if (this == &otra) return *this;
    delete[] celdas;
    celdas = nullptr;
    n = otra.n;
    size_t total = (size_t)n * n;
    if (total > 0) {
        celdas = new char[total];
        for (size_t k = 0; k < total; k++) celdas[k] = otra.celdas[k];
    }
    return *this;
}

MatrizAmistad::~MatrizAmistad() {
    delete[] celdas;
}

void MatrizAmistad::redimensionar(int nuevoN) {
    delete[] celdas;
    celdas = nullptr;
    n = nuevoN;
    size_t total = (size_t)n * n;
    if (total > 0) {
        celdas = new char[total];
        for (size_t k = 0; k < total; k++) celdas[k] = 0;
    }
}

void MatrizAmistad::agregarAmistad(int i, int j) {
    if (i < 0 || j < 0 || i >= n || j >= n || i == j) return;
    celdas[(size_t)i * n + j] = 1;
    celdas[(size_t)j * n + i] = 1;
}

bool MatrizAmistad::sonAmigos(int i, int j) const {
    if (i < 0 || j < 0 || i >= n || j >= n) return false;
    return celdas[(size_t)i * n + j] != 0;
}

// Producto punto de las filas i y j: cuenta los k donde ambos son amigos de k.
int MatrizAmistad::amigosEnComun(int i, int j, long& iteraciones) const {
    if (i < 0 || j < 0 || i >= n || j >= n) return 0;
    int comunes = 0;
    const char* filaI = celdas + (size_t)i * n;
    const char* filaJ = celdas + (size_t)j * n;
    for (int k = 0; k < n; k++) {
        iteraciones++;
        if (filaI[k] && filaJ[k]) comunes++;
    }
    return comunes;
}

size_t MatrizAmistad::memoria() const {
    return sizeof(*this) + (size_t)n * n;
}