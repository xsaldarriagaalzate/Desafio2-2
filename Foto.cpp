#include "Foto.h"
#include "Cadena.h"

using namespace std;

Foto::Foto() : tamano(0), likes(0) {}

Foto::Foto(const char* fechaCaptura, long tamano, const char* rutaArchivo, int likes)
    : fechaCaptura(fechaCaptura), tamano(tamano), rutaArchivo(rutaArchivo), likes(likes) {}

void Foto::mostrar() const {
    cout << rutaArchivo.cadena_char()
    << "  (likes: " << likes << ")\n";
}