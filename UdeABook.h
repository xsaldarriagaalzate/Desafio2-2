#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <fstream>
#include "Lista.h"
#include "Usuario.h"

using namespace std;

class UdeABook {
private:

    Lista<Usuario> usuarios;
    bool leerLinea(ifstream& archivo, Cadena& linea);
    char** separarCampos(const char* linea, int& cantidad);
    void liberarCampos(char** campos, int cantidad);
    bool cargarUsuarios(const char* ruta);
    bool cargarAmistades(const char* ruta);


public:

    UdeABook();
    bool cargarDatos();
    void verificarCarga() const;

};

#endif // UDEABOOK_H