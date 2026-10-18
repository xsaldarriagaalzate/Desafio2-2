#include "Usuario.h"

using namespace std;

Usuario::Usuario() {}

Usuario::Usuario(const Cadena& nombreUsuario, const Cadena& clave, const Cadena& correo,
                 const Cadena& fechaNacimiento, const Cadena& rutaFotoPerfil,
                 const Cadena& ciudad, const Cadena& pais, const Cadena& fechaInscripcion)
    : nombreUsuario(nombreUsuario), clave(clave), correo(correo),
    fechaNacimiento(fechaNacimiento), rutaFotoPerfil(rutaFotoPerfil),
    ciudad(ciudad), pais(pais), fechaInscripcion(fechaInscripcion) {}

bool Usuario::operator==(const Usuario& otro) const {
    return nombreUsuario == otro.nombreUsuario;
}

void Usuario::mostrar() const {
    cout << "[" << nombreUsuario.cadena_char() << "] [" << clave.cadena_char() << "] [" << correo.cadena_char() << "] [" << fechaNacimiento.cadena_char() << "]\n"
         << "  foto: [" << rutaFotoPerfil.cadena_char() << "]\n"
         << "  [" << ciudad.cadena_char() << "] [" << pais.cadena_char() << "] [" << fechaInscripcion.cadena_char() << "]\n";
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