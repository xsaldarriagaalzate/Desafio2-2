#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include "Cadena.h"
#include "Lista.h"
#include "Fecha.h"

class Usuario {
private:
    Cadena nombreUsuario;
    Cadena clave;
    Cadena correo;
    Fecha fechaNacimiento;
    Cadena rutaFotoPerfil;
    Cadena ciudad;
    Cadena pais;
    Fecha fechaInscripcion;
    Lista<int> amigos;
    Lista<int> solicitudes;
    Lista<int> enviados;
    Lista<int> recibidos;

public:
    Usuario();
    Usuario(const Cadena& nombreUsuario, const Cadena& clave, const Cadena& correo,const Cadena& fechaNacimiento,
            const Cadena& rutaFotoPerfil,
            const Cadena& ciudad, const Cadena& pais, const Cadena& fechaInscripcion);

    const char* getNombreUsuario() {return nombreUsuario.cadena_char();}
    const char* getClave() {return clave.cadena_char();}
    const char* getCorreo() {return correo.cadena_char();}
    const char* getFechaNacimiento() {return fechaNacimiento.cadena_char();}
    const char* getRutaFotoPerfil() {return rutaFotoPerfil.cadena_char();}
    const char* getCiudad() {return ciudad.cadena_char();}
    const char* getPais() {return pais.cadena_char();}
    const char* getFechaInscripcion() {return fechaInscripcion.cadena_char();}

    int cantidadAmigos() const { return amigos.size(); }
    int getAmigo(int i) const { return amigos[i]; }
    bool esAmigo(int indice) const;
    void agregarAmigo(int indice) { amigos.agregar(indice); }

    void agregarSolicitud(int indice) { solicitudes.agregar(indice); }
    bool tieneSolicitudDe(int indice) const;
    int cantidadSolicitudes() const { return solicitudes.size(); }
    int getSolicitud(int k) const { return solicitudes[k]; }

    void agregarEnviado(int indice) { enviados.agregar(indice); }
    void agregarRecibido(int indice) { recibidos.agregar(indice); }
    int cantidadEnviados() const { return enviados.size(); }
    int cantidadRecibidos() const { return recibidos.size(); }
    int getEnviado(int k) const { return enviados[k]; }
    int getRecibido(int k) const { return recibidos[k]; }

    bool operator==(const Usuario& otro) const;

    void mostrar() const;
};

#endif // USUARIO_H