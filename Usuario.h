#ifndef USUARIO_H
#define USUARIO_H

#include "Cadena.h"

class Usuario {
private:
    Cadena nombreUsuario;
    Cadena clave;
    Cadena correo;
    Cadena fechaNacimiento;
    Cadena rutaFotoPerfil;
    Cadena ciudad;
    Cadena pais;
    Cadena fechaInscripcion;

public:
    Usuario();
    Usuario(const char* nombreUsuario, const char* clave, const char* correo,const char* fechaNacimiento,
            const char* rutaFotoPerfil,
            const char* ciudad, const char* pais, const char* fechaInscripcion);

    const char* getNombreUsuario() {return nombreUsuario.cadena_char();}
    const char* getClave() {return clave.cadena_char();}
    const char* getCorreo() {return correo.cadena_char();}
    const char* getFechaNacimiento() {return fechaNacimiento.cadena_char();}
    const char* getRutaFotoPerfil() {return rutaFotoPerfil.cadena_char();}
    const char* getCiudad() {return ciudad.cadena_char();}
    const char* getPais() {return pais.cadena_char();}
    const char* getFechaInscripcion() {return fechaInscripcion.cadena_char();}

    bool operator==(const Usuario& otro) const;

    void mostrar() const;
};

#endif // USUARIO_H