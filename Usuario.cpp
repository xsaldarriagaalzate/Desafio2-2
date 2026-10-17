#include <iostream>
#include "Usuario.h"

using namespace std;

Usuario::Usuario();

Usuario::Usuario(const char* nombreUsuario, const char* clave, const char* correo,
                 const char* fechaNacimiento, const char* rutaFotoPerfil,
                 const char* ciudad, const char* pais, const char* fechaInscripcion)
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