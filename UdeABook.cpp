#include <iostream>
#include <fstream>
#include "UdeABook.h"

using namespace std;

char* UdeABook::leerLinea(ifstream& linea) {

    int capacidad = 0, n = 0;
    char* temporal = new char[capacidad];
    char c;

    while (linea.get(c) && c != '\n') {
        if (c == '\n') continue;
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

    if (n == 0) {
        delete[] temporal;
        return nullptr;
    }
    temporal[n] = '\0';
    return temporal;

}

static bool UdeABook::finLinea(char c) {
    return c == ';' || c == '}' || c == '\0';
}

char** UdeABook::separarCampos(const char* linea, int& cantidad) {

    cantidad = 1;
    int i = 1;

    while (!finLinea(linea[i])) {
        if (linea[i] == ',') cantidad++;
        i++;
    }

    char** campos = new char*[cantidad];

    i = 1;

    for (int c = 0; c < cantidad; c++) {
        int inicial = i;
        while (!finLinea(linea[i]) && linea[i] != ',') {
            i++;
        }
        int len = i - inicial;
        campos[c] = new char[len+1];
        for (int j = 0; j < len; j++) {
            campos[c][j] = linea[inicial+j];
        }
        campos[c][len] = '\0';
        i++;
    }

    return campos;
}

void UdeABook::liberarCampos(char** campos, int cantidad) {
    for (int c = 0; c < cantidad; c++) {
        delete[] campos[c];
    }
    delete[] campos;
}


bool UdeABook::cargarUsuarios(const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    char* linea1;
    while ((linea1 = leerLinea(archivo)) != nullptr) {
        char* linea2 = leerLinea(archivo);
        char* linea3 = leerLinea(archivo);

        if (linea2 == nullptr || linea3 == nullptr) {   // usuario incompleto al final
            delete[] linea1;
            delete[] linea2;
            delete[] linea3;
            break;
        }

        int n1 = 0, n2 = 0, n3 = 0;
        char** campos1 = separarCampos(linea1, n1);
        char** campos2 = separarCampos(linea2, n2);
        char** campos3 = separarCampos(linea3, n3);

        if (n1 == 4 && n2 == 1 && n3 == 3) {
            usuarios.agregar(Usuario(campos1[0], campos1[1], campos1[2], campos1[3],
                                     campos2[0],
                                     campos3[0], campos3[1], campos3[2]));
        }

        liberarCampos(campos1, n1);
        liberarCampos(campos2, n2);
        liberarCampos(campos3, n3);
        delete[] linea1;
        delete[] linea2;
        delete[] linea3;
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