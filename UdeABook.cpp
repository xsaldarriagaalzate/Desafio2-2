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

bool UdeABook::finLinea(char c) {
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

        Lista<Cadena> campos1 = separarCampos(linea1);
        Lista<Cadena> campos2 = separarCampos(linea2);
        Lista<Cadena> campos3 = separarCampos(linea3);

        if (campos1.tamanio() == 4 && campos2.tamanio() == 1 && campos3.tamanio() == 3) {
            usuarios.agregar(Usuario(campos1[0], campos1[1], campos1[2], campos1[3],
                                     campos2[0],
                                     campos3[0], campos3[1], campos3[2]));
        }
    }
    return true;
}

bool UdeABook::separarCamposAmistades(const Cadena &linea, Cadena dueno, Lista<Cadena> amigos) {
    const char* texto = linea.cadena_char();
    int i = 0;
    if (texto[i] == '*') i++;

    int ini = i;
    while (texto[i] != ':' && texto[i] != '\0') i++;
    if (texto[i] != ':') return false;
    dueno = Cadena(texto + ini, i - ini);
    i++;

    while (texto[i] != ';' && texto[i] != '\0') {
        ini = i;
        while (texto[i] != ',' && texto[i] != ';' && texto[i] != '\0') i++;
        if (i > ini) amigos.agregar(Cadena(texto + ini, i - ini));
        if (texto[i] == ',') i++;
    }
    return true;
}

int UdeABook::buscarUsuario(const Cadena& nombre) const {
    for (int i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].getNombre() == nombre) return i;
    }
    return -1;
}

bool UdeABook::cargarAmistades(const char *ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena linea, dueno;
    while (leerLinea(archivo,linea)) {
        Lista<Cadena> amigos;

        if(!separarCamposAmistades(archivo,dueno,amigos)) continue;

        int a = buscarUsuario(dueno);
        if (a < 0) continue;

        for (int k = 0; k < amigos.size(); k++) {
            int b = buscarUsuario(amigos[k]);
            if (b < 0 || b == a) continue;

            if (!usuarios[a].esAmigo(b)) usuarios[a].agregarAmigo(b);
            if (!usuarios[b].esAmigo(a)) usuarios[b].agregarAmigo(a);
        }
    }
    return true;
}

bool UdeABook::cargarSolicitudes(const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena linea;
    while (leerLinea(archivo, linea)) {
        const char* texto = linea.cadena_char();

        int coma = 0;
        while (texto[coma] != ',' && texto[coma] != '\0') coma++;
        if (texto[coma] != ',') continue;

        int fin = coma + 1;
        while (texto[fin] != ';' && texto[fin] != '\0') fin++;

        Cadena emisor(texto, coma);
        Cadena receptor(texto + coma + 1, fin - coma - 1);

        int e = buscarUsuario(emisor);
        int r = buscarUsuario(receptor);
        if (e < 0 || r < 0 || e == r) continue;
        if (usuarios[r].esAmigo(e)) continue;
        if (usuarios[r].tieneSolicitudDe(e)) continue;
        if (usuarios[e].tieneSolicitudDe(r)) continue;

        usuarios[r].agregarSolicitud(e);
    }
    return true;
}

bool UdeABook::cargarMensajes(const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena lineaDatos, lineaTexto;
    while (leerLinea(archivo, lineaDatos)) {
        if (!leerLinea(archivo, lineaTexto)) break;

        Lista<Cadena> campos = separarCampos(lineaDatos);
        if (campos.size() != 4) continue;

        const char* t = lineaTexto.cadena_char();
        int n = lineaTexto.largo();
        if (n < 3 || t[0] != '{' || t[n - 1] != '}') continue;
        Cadena texto(t + 1, n - 2);

        int e = buscarUsuario(campos[0]);
        int r = buscarUsuario(campos[1]);
        if (e < 0 || r < 0 || e == r) continue;
        if (!usuarios[e].esAmigo(r)) continue;
        Fecha f;
        if (!f.desdeTexto(campos[2].cadena_char())) continue;
        if (!Mensaje::textoValido(texto.cadena_char())) continue;
        bool leido = (campos[3].largo() == 1 && campos[3].cadena_char()[0] == '1');

        int indice = mensajes.size();
        mensajes.agregar(Mensaje(e, r, texto.cadena_char(), f, leido));
        usuarios[e].agregarEnviado(indice);
        usuarios[r].agregarRecibido(indice);
    }
    return true;
}

bool UdeABook::cargarDatos() {
    return cargarUsuarios("usuarios.txt")
        && cargarAmistades("amistades.txt")
        && cargarSolicitudes("solicitudes.txt")
        && cargarMensajes("mensajes.txt");
}

void UdeABook::verificarCarga() const {
    cout << "Usuarios cargados: " << usuarios.size() << "\n";
    cout << "Mensajes cargados: " << mensajes.size() << "\n\n";

    for (int i = 0; i < usuarios.size(); i++) {
        const Usuario& u = usuarios[i];

        cout << i << ":\n";
        u.mostrar();

        cout << "  amigos (" << u.cantidadAmigos() << "):";
        for (int k = 0; k < u.cantidadAmigos(); k++)
            cout << " " << usuarios[u.getAmigo(k)].getNombre().cadena_char();
        cout << "\n";

        cout << "  solicitudes recibidas (" << u.cantidadSolicitudes() << "):";
        for (int k = 0; k < u.cantidadSolicitudes(); k++)
            cout << " " << usuarios[u.getSolicitud(k)].getNombre().cadena_char();
        cout << "\n";

        cout << "  mensajes enviados: " << u.cantidadEnviados()
             << " | recibidos: " << u.cantidadRecibidos() << "\n\n";
    }

    cout << "--- Mensajes ---\n";
    for (int i = 0; i < mensajes.size(); i++) {
        const Mensaje& m = mensajes[i];
        m.mostrarCon(usuarios[m.getEmisor()].getNombre().cadena_char(),
                     usuarios[m.getReceptor()].getNombre().cadena_char());
    }
}