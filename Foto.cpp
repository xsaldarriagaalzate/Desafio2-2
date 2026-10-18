#include "Foto.h"

using namespace std;

Foto::Foto() : tamano(0), likes(0) {}

Foto::Foto(const char* fechaCaptura, long tamano, const char* rutaArchivo, int likes)
    : fechaCaptura(fechaCaptura), tamano(tamano), rutaArchivo(rutaArchivo), likes(likes) {}

bool Foto::leGusta(int usuario) const {
    for (int k = 0; k < likers.size(); k++)
        if (likers[k] == usuario) return true;
    return false;
}

bool Foto::alternarLike(int usuario) {
    for (int k = 0; k < likers.size(); k++) {
        if (likers[k] == usuario) {
            likers.eliminarEn(k);
            if (likes > 0) likes--;
            return false;
        }
    }
    likers.agregar(usuario);
    likes++;
    return true;
}

void Foto::agregarLiker(int usuario) {      // se usa solo al cargar el archivo
    if (leGusta(usuario)) return;
    likers.agregar(usuario);
    if (likers.size() > likes) likes = likers.size();
}

size_t Foto::memoriaDinamica() const {
    return fechaCaptura.memoriaDinamica() + rutaArchivo.memoriaDinamica() + likers.memoriaArreglo();
}

void Foto::mostrar() const {
    cout << rutaArchivo.cadena_char()
    << "  (likes: " << likes << ")\n";
}