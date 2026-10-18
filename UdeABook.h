#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <fstream>
#include "Lista.h"
#include "Cadena.h"
#include "Usuario.h"
#include "Fecha.h"
#include "Mensaje.h"
#include "MatrizAmistad.h"

using namespace std;

class UdeABook {
private:

    Lista<Usuario> usuarios;
    Lista<Mensaje> mensajes;
    MatrizAmistad amistades;

    static const int CAMBIOS_PARA_GUARDAR = 2;
    int cambiosPendientes;
    long iteraciones;
    int usuarioEnSesion;

    int mejoresAmigos[3];
    int cantidadMejores;

    bool leerLinea(ifstream& archivo, Cadena& linea);
    Lista<Cadena> separarCampos(const Cadena& linea);
    bool finLinea(char c);
    bool separarCamposAmistades(const Cadena& linea, Cadena& dueno, Lista<Cadena>& amigos);

    bool cargarUsuarios(const char* ruta);
    bool cargarAmistades(const char* ruta);
    bool cargarSolicitudes(const char* ruta);
    bool cargarMensajes(const char* ruta);
    bool cargarAlbumes(const char* ruta);

    int buscarUsuario(const Cadena& nombre);

    bool registrarAmistad(int i, int j);

    void administrarAmistades();
    void enviarSolicitudAmistad();
    void aceptarSolicitudes();
    int  enviarSolicitud(int destino);
    void visitarPerfil();
    void verSugerencias();
    void administrarMensajes();
    void enviarMensaje();
    void leerMensajesNuevos();
    void buscarMensajesPorFecha();

    bool guardarAlbumes(const char* ruta) const;
    size_t carrusel(Album& album);
    bool guardarAmistades(const char* ruta) const;
    bool guardarSolicitudes(const char* ruta) const;
    bool guardarMensajes(const char* ruta) const;
    void guardarPendientes();
    void cerrarSesion();


public:
    UdeABook();
    bool cargarDatos();
    void verificarCarga() const;
    size_t memoriaTotal() const;
    void mostrarMejoresAmigos();
    void reportarRecursos(size_t memoriaLocal = 0) const;
    void menuPrincipal();
    bool iniciarSesion();
    bool guardarDatos() const;

};

#endif // UDEABOOK_H