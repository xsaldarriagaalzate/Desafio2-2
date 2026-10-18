#include "Usuario.h"

using namespace std;

Usuario::Usuario() {}

Usuario::Usuario(const Cadena& nombreUsuario, const Cadena& clave, const Cadena& correo,
                 const Fecha& fechaNacimiento, const Cadena& rutaFotoPerfil,
                 const Cadena& ciudad, const Cadena& pais, const Fecha& fechaInscripcion)
    : nombreUsuario(nombreUsuario), clave(clave), correo(correo),
    fechaNacimiento(fechaNacimiento), rutaFotoPerfil(rutaFotoPerfil),
    ciudad(ciudad), pais(pais), fechaInscripcion(fechaInscripcion) {}

bool Usuario::operator==(const Usuario& otro) const {
    return nombreUsuario == otro.nombreUsuario;
}

bool Usuario::esAmigo(int indice) const {
    for (int i = 0; i < amigos.size(); i++)
        if (amigos[i] == indice) return true;
    return false;
}

bool Usuario::tieneSolicitudDe(int indice) const {
    for (int i = 0; i < solicitudes.size(); i++)
        if (solicitudes[i] == indice) return true;
    return false;
}

void Usuario::mostrar() const {
    cout << "[" << nombreUsuario.cadena_char() << "] [" << clave.cadena_char() << "] [" << correo.cadena_char() << "] [" << fechaNacimiento.aCadena().cadena_char() << "]\n"
         << "  foto: [" << rutaFotoPerfil.cadena_char() << "]\n"
         << "  [" << ciudad.cadena_char() << "] [" << pais.cadena_char() << "] [" << fechaInscripcion.aCadena().cadena_char() << "]\n";
}

size_t Usuario::memoriaDinamica() const {
    size_t total = albumes.memoriaArreglo();
    for (int i = 0; i < albumes.size(); i++)
        total += albumes[i].memoriaDinamica();
    Album albumVacio;
    total += (size_t)(albumes.capacidad() - albumes.size()) * albumVacio.memoriaDinamica();
    return total + nombreUsuario.memoriaDinamica() + clave.memoriaDinamica() +
           correo.memoriaDinamica() + rutaFotoPerfil.memoriaDinamica() +
           ciudad.memoriaDinamica() + pais.memoriaDinamica() +
           amigos.memoriaArreglo() + solicitudes.memoriaArreglo() +
           enviados.memoriaArreglo() + recibidos.memoriaArreglo();
}
