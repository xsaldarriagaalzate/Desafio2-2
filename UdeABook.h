#ifndef UDEABOOK_H
#define UDEABOOK_H

#include <fstream>
#include "Lista.h"
#include "Usuario.h"

class UdeABook {
private:

    Lista<Usuario> usuarios;
    char* leerLinea(std::ifstream& archivo);
    char** separarCampos(const char* linea, int& cantidad);
    void liberarCampos(char** campos, int cantidad);
    bool cargarUsuarios(const char* ruta);


public:

    UdeABook();
    bool cargarDatos();
    void verificarCarga() const;

};

#endif // UDEABOOK_H