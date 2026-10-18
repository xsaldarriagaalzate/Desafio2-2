#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <fstream>
#include "Lista.h"
#include "Usuario.h"
#include "Fecha.h"
#include "Mensaje.h"

using namespace std;

class UdeABook {
private:

    Lista<Usuario> usuarios;
    bool leerLinea(ifstream& archivo, Cadena& linea);
    Lista<Cadena> separarCampos(const Cadena& linea);
    bool cargarUsuarios(const char* ruta);
    Lista<Cadena> separarCamposAmistades(const Cadena& linea, Cadena dueno, Lista<Cadena> amigos);
    bool cargarAmistades(const char* ruta);
    bool cargarSolicitudes(const char* ruta);
    bool finLinea(char c);
    int buscarUsuario(const Cadena& nombre) const;


public:



    UdeABook();
    bool cargarDatos();
    void verificarCarga() const;

};

#endif // UDEABOOK_H