#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <fstream>
#include "Lista.h"
#include "Cadena.h"
#include "Usuario.h"
#include "Fecha.h"
#include "Mensaje.h"

using namespace std;

class UdeABook {
private:

    Lista<Usuario> usuarios;
    Lista<Mensaje> mensajes;

    bool leerLinea(ifstream& archivo, Cadena& linea);
    Lista<Cadena> separarCampos(const Cadena& linea);
    bool finLinea(char c);
    bool separarCamposAmistades(const Cadena& linea, Cadena& dueno, Lista<Cadena> amigos);

    bool cargarUsuarios(const char* ruta);
    bool cargarAmistades(const char* ruta);
    bool cargarSolicitudes(const char* ruta);
    bool cargarMensajes(const char* ruta);

    int buscarUsuario(const Cadena& nombre) const;


public:
    UdeABook();
    bool cargarDatos();
    void verificarCarga() const;

    void menuPrincipal();

};

#endif // UDEABOOK_H