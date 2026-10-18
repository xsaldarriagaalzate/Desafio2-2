#include "Album.h"

Album::Album() : privacidad(PRIVADO) {}

Album::Album(const char* nombre, const char* ruta, Privacidad privacidad)
    : nombre(nombre), ruta(ruta), privacidad(privacidad) {}

bool Album::esVisiblePara(bool esDueno, bool esAmigo) const {
    if (privacidad == PUBLICO) return true;
    if (privacidad == AMIGOS) return esDueno || esAmigo;
    return esDueno;
}


int Album::indiceSiguiente(int actual) const {
    int n = fotos.size();
    if (n == 0) return -1;
    return (actual + 1) % n;
}

int Album::indicePrevio(int actual) const {
    int n = fotos.size();
    if (n == 0) return -1;
    return (actual - 1 + n) % n;
}

Album::Privacidad Album::privacidadDesdeTexto(const char* texto) {
    Cadena t(texto);
    if (t == Cadena("publico")) return PUBLICO;
    if (t == Cadena("amigos")) return AMIGOS;
    return PRIVADO;
}

const char* Album::privacidadATexto() const {
    if (privacidad == PUBLICO) return "publico";
    if (privacidad == AMIGOS) return "amigos";
    return "privado";
}

void Album::mostrar() const {
    cout << nombre.cadena_char() << " [" << privacidadATexto() << "] - "
              << fotos.size() << " fotos\n";
}
