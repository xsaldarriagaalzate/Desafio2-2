#include <iostream>
#include <fstream>
#include "UdeABook.h"

using namespace std;

bool UdeABook::leerLinea(ifstream& archivo, Cadena& linea) {

    int capacidad = 16, n = 0;
    char* temporal = new char[capacidad];
    char c;

    while (archivo.get(c) && c != '\n') {
        if (c == '\r') continue;
        if (n+1 == capacidad) {
            capacidad *= 2;
            char* nuevo = new char[capacidad];
            for (int i = 0; i < n; ++i) {
                nuevo[i] = temporal[i];
            }
            delete[] temporal;
            temporal = nuevo;
        }
        temporal[n++] = c;
    }

    if (n == 0 && archivo.eof()) {
        delete[] temporal;
        return false;
    }

    linea = Cadena(temporal,n);

    delete[] temporal;
    return true;

}

static bool UdeABook::finLinea(char c) {
    return c == ';' || c == '}' || c == '\0';
}

Lista<Cadena> UdeABook::separarCampos(const Cadena& linea) {

    Lista<Cadena> campos;
    if (linea.largo() == 0) return campos;

    const char* texto = linea.cadena_char();
    int i = 1;
    while (true) {
        int inicial = i;
        while (!finLinea(texto[i]) && texto[i] != ',') {
            i++;
        }
        campos.agregar(Cadena(texto + inicial, i - inicial));
        if (finLinea(texto[i])) break;
        i++;
    }
    return campos;
}

bool UdeABook::cargarUsuarios(const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena linea1, linea2, linea3;
    while (leerLinea(archivo,linea1)) {

        if (!leerLinea(archivo,linea2) || !leerLinea(archivo,linea3)) break;

        Lista<Cadena> campos1 = separarCampos(linea1, n1);
        Lista<Cadena> campos2 = separarCampos(linea2, n2);
        Lista<Cadena> campos3 = separarCampos(linea3, n3);

        if (campos1.tamanio() == 4 && campos2.tamanio() == 1 && campos3.tamanio() == 3) {
            usuarios.agregar(Usuario(campos1[0], campos1[1], campos1[2], campos1[3],
                                     campos2[0],
                                     campos3[0], campos3[1], campos3[2]));
        }
    }
    return true;
}

bool UdeABook::cargarDatos() {
    cargarUsuarios("usuarios.txt");
    if (cargarUsuarios("usuarios.txt")) {
        return true;
    }
    else return false;
}

void UdeABook::verificarCarga() const {
    cout << "Usuarios cargados: " << usuarios.size() << "\n";
    for (int i = 0; i < usuarios.size(); i++) {
        cout << i << ":\n";
        usuarios[i].mostrar();
    }
}