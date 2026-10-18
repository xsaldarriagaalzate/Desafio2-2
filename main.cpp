#include <iostream>
#include "UdeABook.h"
using namespace std;

int main() {
    UdeABook app;

    if (!app.cargarDatos()) {
        cout << "No se pudieron cargar los datos\n";
        return 1;
    }

    while (app.iniciarSesion()) {
        app.menuPrincipal();
    }

    cout << "Hasta pronto\n";
    return 0;
}