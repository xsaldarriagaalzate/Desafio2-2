#include "Foto.h"
#include <iostream>

Foto::Foto() : tamano(0), likes(0) {}

Foto::Foto(const char* fechaCaptura, long tamano, const char* rutaArchivo, int likes)
    : fechaCaptura(fechaCaptura), tamano(tamano), rutaArchivo(rutaArchivo), likes(likes) {}

void Foto::mostrar() const {
    std::cout << rutaArchivo.c_str()
    << "  (likes: " << likes << ")\n";
}

size_t Foto::memoriaDinamica() const {
    return (fechaCaptura.largo() + 1) + (rutaArchivo.largo() + 1);
}