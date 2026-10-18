#include <iostream>
#include <fstream>
#include "UdeABook.h"
#include <random>
#include <chrono>

using namespace std;

static void leerLineaConsola(char* destino, int max) {
    cin.getline(destino, max);
    if (cin.fail() && !cin.eof()) { cin.clear(); cin.ignore(100000, '\n'); }
    if (cin.eof()) destino[0] = '\0';
}

static char leerOpcion() {
    char b[16];
    leerLineaConsola(b, 16);
    if (cin.eof()) return 's';
    return b[0];
}

static int leerEntero() {
    char b[16];
    leerLineaConsola(b, 16);
    if (b[0] == '\0') return -1;
    int n = 0;
    for (int i = 0; b[i] != '\0'; i++) {
        if (b[i] < '0' || b[i] > '9' || i > 8) return -1;
        n = n * 10 + (b[i] - '0');
    }
    return n;
}

static void avisoSolicitud(int codigo) {
    switch (codigo) {
    case 0: cout << "Solicitud enviada\n"; break;
    case 1: cout << "Ese usuario no existe\n"; break;
    case 2: cout << "Ya son amigos\n"; break;
    case 3: cout << "Ese usuario ya te envio una solicitud; aceptala desde Administrar amistades\n"; break;
    case 4: cout << "Ya le enviaste una solicitud\n"; break;
    }
}

static mt19937 generador((unsigned)chrono::steady_clock::now().time_since_epoch().count());

bool UdeABook::finLinea(char c) {
    return c == ';' || c == '}' || c == '\0';
}

UdeABook::UdeABook() : cambiosPendientes(0), iteraciones(0), usuarioEnSesion(-1), cantidadMejores(0) {
    for (int k = 0; k < 3; k++) mejoresAmigos[k] = -1;
}

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

        if (!(campos1.size() == 4 && campos2.size() == 1 && campos3.size() == 3)) continue;

        Fecha nacimiento, inscripcion;
        if (!nacimiento.desdeTexto(campos1[3].cadena_char()) ||
            !inscripcion.desdeTexto(campos3[2].cadena_char())) continue;

        usuarios.agregar(Usuario(campos1[0], campos1[1], campos1[2], nacimiento,
                                 campos2[0],
                                 campos3[0], campos3[1], inscripcion));
    }
    return true;
}

bool UdeABook::registrarAmistad(int i, int j) {
    int n = usuarios.size();
    if (i < 0 || j < 0 || i >= n || j >= n || i == j) return false;
    if (amistades.sonAmigos(i, j)) return false;
    amistades.agregarAmistad(i, j);
    usuarios[i].agregarAmigo(j);
    usuarios[j].agregarAmigo(i);
    return true;
}

int UdeABook::buscarUsuario(const Cadena& nombre) {
    for (int k = 0; k < usuarios.size(); k++) {
        iteraciones++;
        if (usuarios[k].getNombre() == nombre) return k;
    }
    return -1;
}

bool UdeABook::separarCamposAmistades(const Cadena &linea, Cadena& dueno, Lista<Cadena>& amigos) {
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

bool UdeABook::cargarAmistades(const char *ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena linea, dueno;
    while (leerLinea(archivo,linea)) {
        Lista<Cadena> amigos;

        if(!separarCamposAmistades(linea,dueno,amigos)) continue;

        int a = buscarUsuario(dueno);
        if (a < 0) continue;

        for (int k = 0; k < amigos.size(); k++) {
            int b = buscarUsuario(amigos[k]);
            if (b < 0 || b == a) continue;

            registrarAmistad(a,b);
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

static bool aEnteroLargo(const Cadena& c, long& valor) {
    const char* t = c.cadena_char();
    if (c.largo() == 0 || c.largo() > 12) return false;
    valor = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        if (t[i] < '0' || t[i] > '9') return false;
        valor = valor * 10 + (t[i] - '0');
    }
    return true;
}

bool UdeABook::cargarAlbumes(const char* ruta) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) return false;

    Cadena linea;
    int dueno = -1;      // usuario del album actual (-1 = album invalido)
    int album = -1;      // posicion del album actual dentro de ese usuario

    while (leerLinea(archivo, linea)) {
        if (linea.largo() == 0) continue;
        char tipo = linea.cadena_char()[0];
        Lista<Cadena> campos = separarCampos(linea);

        if (tipo == '*') {
            dueno = -1;
            if (campos.size() != 4) continue;
            int u = buscarUsuario(campos[0]);
            if (u < 0) continue;
            usuarios[u].agregarAlbum(Album(campos[1].cadena_char(), campos[2].cadena_char(),
                                           Album::privacidadDesdeTexto(campos[3].cadena_char())));
            dueno = u;
            album = usuarios[u].cantidadAlbumes() - 1;
        } else if (tipo == '{') {
            if (dueno < 0 || campos.size() < 4) continue;
            Fecha captura;
            long tamano, likes;
            if (!captura.desdeTexto(campos[0].cadena_char())) continue;
            if (!aEnteroLargo(campos[1], tamano) || !aEnteroLargo(campos[3], likes)) continue;

            Foto f(campos[0].cadena_char(), tamano, campos[2].cadena_char(), (int)likes);
            for (int k = 4; k < campos.size(); k++) {
                int u = buscarUsuario(campos[k]);
                if (u >= 0) f.agregarLiker(u);
            }
            usuarios[dueno].getAlbum(album).agregarFoto(f);
        }
    }
    return true;
}

size_t UdeABook::memoriaTotal() const {
    size_t total = sizeof(*this);

    // 2. Lista de usuarios
    total += usuarios.memoriaArreglo();
    for (int i = 0; i < usuarios.size(); i++) {
        total += usuarios[i].memoriaDinamica();
    }
    Usuario vacio;
    total += (size_t)(usuarios.capacidad() - usuarios.size()) * vacio.memoriaDinamica();

    total += amistades.memoriaDinamica();

    total += sizeof(generador);

    total += mensajes.memoriaArreglo();
    for (int i = 0; i < mensajes.size(); i++) {
        total += mensajes[i].memoriaDinamica();
    }
    Mensaje mensajeVacio;
    total += (size_t)(mensajes.capacidad() - mensajes.size()) * mensajeVacio.memoriaDinamica();

    return total;
}

bool UdeABook::cargarDatos() {
    if (!cargarUsuarios("usuarios.txt")) return false;

    amistades.redimensionar(usuarios.size());

    return cargarAmistades("amistades.txt")
           && cargarSolicitudes("solicitudes.txt")
           && cargarMensajes("mensajes.txt");
           && cargarAlbumes("albumes.txt");
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

    cout << "[Iteraciones: " << iteraciones << " | Memoria: " << memoriaTotal() << " bytes]\n";
}

static int pesoPorComunes(int comunes) {
    if (comunes <= 10) return 2;
    if (comunes <= 20) return 5;
    return 7;
}

void UdeABook::mostrarMejoresAmigos() {
    iteraciones = 0;
    if (usuarioEnSesion < 0 || usuarioEnSesion >= usuarios.size()) {
        cout << "No hay una sesion iniciada\n";
        return;
    }

    const Usuario& yo = usuarios[usuarioEnSesion];
    Lista<int> candidatos;
    Lista<int> pesos;
    for (int k = 0; k < yo.cantidadAmigos(); k++) {
        iteraciones++;
        int j = yo.getAmigo(k);
        int comunes = amistades.amigosEnComun(usuarioEnSesion, j, iteraciones);
        candidatos.agregar(j);
        pesos.agregar(pesoPorComunes(comunes));
    }

    if (candidatos.size() >= 6) {
        for (int c = 0; c < candidatos.size(); c++) {
            for (int k = 0; k < cantidadMejores; k++) {
                iteraciones++;
                if (candidatos[c] == mejoresAmigos[k]) pesos[c] = 0;
            }
        }
    }

    int cuantos = candidatos.size() < 3 ? candidatos.size() : 3;
    cantidadMejores = 0;
    for (int v = 0; v < cuantos; v++) {
        int total = 0;
        for (int c = 0; c < candidatos.size(); c++) {
            iteraciones++;
            total += pesos[c];
        }
        uniform_int_distribution<int> dado(0, total - 1);
        int r = dado(generador);

        int c = 0;
        while (r >= pesos[c]) {
            iteraciones++;
            r -= pesos[c];
            c++;
        }
        mejoresAmigos[cantidadMejores++] = candidatos[c];
        pesos[c] = 0;
    }

    if (cantidadMejores == 0) {
        cout << "Aun no tienes amigos para mostrar\n";
    }
    for (int v = 0; v < cantidadMejores; v++) {
        cout << "Foto perfil de amigo " << (v + 1) << ": "
             << usuarios[mejoresAmigos[v]].getRutaFotoPerfil() << "\n";
    }

    size_t memoriaLocal = sizeof(cuantos) + candidatos.memoriaContenedor() + pesos.memoriaContenedor();
    reportarRecursos(memoriaLocal);
}

void UdeABook::reportarRecursos(size_t memoriaLocal) const {
    cout << "[Iteraciones: " << iteraciones << " | Memoria: "
         << (memoriaTotal() + memoriaLocal) << " bytes]\n";
}

bool UdeABook::iniciarSesion() {
    char nombre[64], clave[64];
    while (true) {
        cout << "\nUsuario (escriba 'salir' para terminar): ";
        leerLineaConsola(nombre, 64);
        if (cin.eof() || Cadena(nombre) == Cadena("salir")) return false;
        cout << "Clave: ";
        leerLineaConsola(clave, 64);

        int u = buscarUsuario(Cadena(nombre));
        if (u >= 0 && usuarios[u].claveCorrecta(Cadena(clave))) {
            usuarioEnSesion = u;
            cantidadMejores = 0;         // sin fotos previas que evitar
            return true;
        }
        cout << "Usuario o clave incorrectos\n";
    }
}

void UdeABook::cerrarSesion() {
    guardarPendientes();
    usuarioEnSesion = -1;
    cantidadMejores = 0;
}

void UdeABook::menuPrincipal() {
    mostrarMejoresAmigos();
    char opcion;
    do {
        cout << "\nBienvenido a UdeaBook " << usuarios[usuarioEnSesion].getNombreUsuario() << "\n\n"
             << "Menu de opciones:\n\n"
             << "1.- Administrar amistades\n"
             << "2.- Visitar perfil de un amigo\n"
             << "3.- Ver sugerencias de amistad\n"
             << "4.- Administrar mensajes\n\n"
             << ".- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n"
             << ".- Ingrese 'S' o 's' para cerrar su sesion.\n\n"
             << "Su opcion: ";
        opcion = leerOpcion();
        switch (opcion) {
        case '1': administrarAmistades(); break;
        case '2': visitarPerfil();        break;
        case '3': verSugerencias();       break;
        case '4': administrarMensajes();  break;
        case 'N': case 'n': mostrarMejoresAmigos(); break;
        case 'S': case 's': break;
        default: cout << "Opcion no valida\n";
        }
        if (cambiosPendientes >= CAMBIOS_PARA_GUARDAR) guardarPendientes();
    } while (opcion != 'S' && opcion != 's');
    cerrarSesion();
}

void UdeABook::administrarAmistades() {
    cout << "\n1.- Enviar solicitud de amistad\n"
         << "2.- Aceptar solicitudes recibidas\n"
         << "0.- Volver\nSu opcion: ";
    char op = leerOpcion();
    if (op == '1') enviarSolicitudAmistad();
    else if (op == '2') aceptarSolicitudes();
}

int UdeABook::enviarSolicitud(int destino) {
    int yo = usuarioEnSesion;
    if (destino < 0 || destino >= usuarios.size() || destino == yo) return 1;
    iteraciones += usuarios[yo].cantidadAmigos() + usuarios[yo].cantidadSolicitudes()
                   + usuarios[destino].cantidadSolicitudes();
    if (amistades.sonAmigos(yo, destino)) return 2;
    if (usuarios[yo].tieneSolicitudDe(destino)) return 3;
    if (usuarios[destino].tieneSolicitudDe(yo)) return 4;
    usuarios[destino].agregarSolicitud(yo);
    cambiosPendientes++;
    return 0;
}

void UdeABook::enviarSolicitudAmistad() {
    iteraciones = 0;
    char nombre[64];
    cout << "Nombre de usuario: ";
    leerLineaConsola(nombre, 64);
    int destino = buscarUsuario(Cadena(nombre));
    avisoSolicitud(enviarSolicitud(destino));
    reportarRecursos(sizeof(nombre) + sizeof(destino));
}

void UdeABook::aceptarSolicitudes() {
    iteraciones = 0;
    Usuario& yo = usuarios[usuarioEnSesion];
    int n = 0;
    if (yo.cantidadSolicitudes() == 0) {
        cout << "No tienes solicitudes pendientes\n";
    } else {
        cout << "Solicitudes recibidas:\n";
        for (int k = 0; k < yo.cantidadSolicitudes(); k++) {
            iteraciones++;
            cout << (k + 1) << ".- " << usuarios[yo.getSolicitud(k)].getNombreUsuario() << "\n";
        }
        cout << "Numero a aceptar (0 para volver): ";
        n = leerEntero();
        if (n >= 1 && n <= yo.cantidadSolicitudes()) {
            int otro = yo.getSolicitud(n - 1);
            registrarAmistad(usuarioEnSesion, otro);
            iteraciones += yo.cantidadSolicitudes();
            yo.eliminarSolicitud(n - 1);
            cambiosPendientes++;
            cout << "Ahora eres amigo de " << usuarios[otro].getNombreUsuario() << "\n";
        }
    }
    reportarRecursos(sizeof(yo) + sizeof(n));
}

void UdeABook::verSugerencias() {
    iteraciones = 0;
    cout << "Cuantas sugerencias desea ver (1-20): ";
    int k = leerEntero();
    if (k < 1 || k > 20) {
        cout << "Valor invalido\n";
        reportarRecursos(sizeof(k));
        return;
    }

    int yo = usuarioEnSesion;
    Lista<int> candidatos, comunes;
    for (int j = 0; j < usuarios.size(); j++) {
        iteraciones++;
        if (j == yo || amistades.sonAmigos(yo, j)) continue;
        candidatos.agregar(j);
        comunes.agregar(amistades.amigosEnComun(yo, j, iteraciones));
    }

    int mostrar = candidatos.size() < k ? candidatos.size() : k;
    int elegidas[20];
    for (int v = 0; v < mostrar; v++) {
        int mejor = -1;
        for (int c = 0; c < candidatos.size(); c++) {
            iteraciones++;
            if (comunes[c] >= 0 && (mejor < 0 || comunes[c] > comunes[mejor])) mejor = c;
        }
        elegidas[v] = candidatos[mejor];
        cout << (v + 1) << ".- " << usuarios[elegidas[v]].getNombreUsuario()
             << " (" << comunes[mejor] << " amigos en comun)\n";
        comunes[mejor] = -1;
    }

    int n = 0;
    if (mostrar == 0) {
        cout << "No hay sugerencias disponibles\n";
    } else {
        cout << "Numero al que desea enviar solicitud (0 para volver): ";
        n = leerEntero();
        if (n >= 1 && n <= mostrar) avisoSolicitud(enviarSolicitud(elegidas[n - 1]));
    }
    reportarRecursos(sizeof(k) + sizeof(yo) + sizeof(mostrar) + sizeof(elegidas) + sizeof(n)
                     + candidatos.memoriaContenedor() + comunes.memoriaContenedor());
}

void UdeABook::administrarMensajes() {
    cout << "\n1.- Enviar mensaje\n"
         << "2.- Leer nuevos mensajes\n"
         << "3.- Buscar mensajes por rango de fecha\n"
         << "0.- Volver\nSu opcion: ";
    char op = leerOpcion();
    if (op == '1') enviarMensaje();
    else if (op == '2') leerMensajesNuevos();
    else if (op == '3') buscarMensajesPorFecha();
}

void UdeABook::enviarMensaje() {
    iteraciones = 0;
    char nombre[64], texto[1024];
    cout << "Nombre del amigo: ";
    leerLineaConsola(nombre, 64);
    int a = buscarUsuario(Cadena(nombre));
    if (a < 0 || !amistades.sonAmigos(usuarioEnSesion, a)) {
        cout << "Ese usuario no es tu amigo\n";
    } else {
        cout << "Mensaje (maximo 512 caracteres): ";
        leerLineaConsola(texto, 1024);
        if (!Mensaje::textoValido(texto)) {
            cout << "Mensaje vacio o de mas de 512 caracteres\n";
        } else {
            int indice = mensajes.size();
            mensajes.agregar(Mensaje(usuarioEnSesion, a, texto));
            usuarios[usuarioEnSesion].agregarEnviado(indice);
            usuarios[a].agregarRecibido(indice);
            cambiosPendientes++;
            cout << "Mensaje enviado\n";
        }
    }
    reportarRecursos(sizeof(nombre) + sizeof(texto) + sizeof(a));
}

void UdeABook::leerMensajesNuevos() {
    iteraciones = 0;
    const Usuario& yo = usuarios[usuarioEnSesion];
    int nuevos = 0;

    for (int k = 0; k < yo.cantidadRecibidos(); k++) {
        iteraciones++;
        Mensaje& m = mensajes[yo.getRecibido(k)];
        if (m.estaLeido()) continue;
        m.mostrarCon(usuarios[m.getEmisor()].getNombreUsuario(), yo.getNombreUsuario());
        m.marcarLeido();
        nuevos++;
    }
    if (nuevos == 0) cout << "No tienes mensajes nuevos\n";
    else cambiosPendientes++;
    reportarRecursos(sizeof(nuevos));
}

void UdeABook::buscarMensajesPorFecha() {
    iteraciones = 0;
    char t1[16], t2[16];
    Fecha ini, fin;
    cout << "Fecha de inicio (dd/mm/aaaa): ";
    leerLineaConsola(t1, 16);
    cout << "Fecha de fin (dd/mm/aaaa): ";
    leerLineaConsola(t2, 16);

    if (!ini.desdeTexto(t1) || !fin.desdeTexto(t2) || ini > fin) {
        cout << "Fechas invalidas\n";
    } else {
        const Usuario& yo = usuarios[usuarioEnSesion];
        int hallados = 0;
        for (int k = 0; k < yo.cantidadRecibidos(); k++) {
            iteraciones++;
            const Mensaje& m = mensajes[yo.getRecibido(k)];
            if (m.getFecha() > fin) break;
            if (m.getFecha() < ini) continue;
            m.mostrarCon(usuarios[m.getEmisor()].getNombreUsuario(), yo.getNombreUsuario());
            hallados++;
        }
        if (hallados == 0) cout << "No hay mensajes en ese rango\n";
    }
    reportarRecursos(sizeof(t1) + sizeof(t2) + sizeof(ini) + sizeof(fin));
}

size_t UdeABook::carrusel(Album& album) {
    if (album.cantidadFotos() == 0) {
        cout << "El album no tiene fotos\n";
        return 0;
    }
    int actual = 0;
    char op;
    do {
        iteraciones++;
        Foto& f = album.getFoto(actual);
        bool gusta = f.leGusta(usuarioEnSesion);
        iteraciones += f.cantidadLikers();

        cout << "\nFoto " << (actual + 1) << " de " << album.cantidadFotos() << "\n"
             << f.getRutaArchivo() << "\n"
             << "Likes: " << f.getLikes() << (gusta ? "  (te gusta)" : "") << "\n\n"
             << "1.- Siguiente\n"
             << "2.- Previa\n"
             << "3.- " << (gusta ? "Quitar like" : "Dar like") << "\n"
             << "0.- Salir del album\n"
             << "Su opcion: ";
        op = leerOpcion();
        if (cin.eof()) op = '0';

        if (op == '1') actual = album.indiceSiguiente(actual);
        else if (op == '2') actual = album.indicePrevio(actual);
        else if (op == '3') {
            f.alternarLike(usuarioEnSesion);
            iteraciones += f.cantidadLikers();
            cambiosPendientes++;
        }
        else if (op != '0') cout << "Opcion no valida\n";
    } while (op != '0');
    return sizeof(actual) + sizeof(op);
}

void UdeABook::visitarPerfil() {
    iteraciones = 0;
    char nombre[64];
    cout << "Nombre de usuario del amigo: ";
    leerLineaConsola(nombre, 64);

    int a = buscarUsuario(Cadena(nombre));
    if (a < 0 || !amistades.sonAmigos(usuarioEnSesion, a)) {
        cout << "Ese usuario no es tu amigo\n";
        reportarRecursos(sizeof(nombre) + sizeof(a));
        return;
    }

    Usuario& amigo = usuarios[a];
    Lista<int> visibles;
    for (int k = 0; k < amigo.cantidadAlbumes(); k++) {
        iteraciones++;
        if (amigo.getAlbum(k).esVisiblePara(false, true)) visibles.agregar(k);
    }

    size_t memoriaCarrusel = 0;
    int n = -1;
    if (visibles.size() == 0) {
        cout << amigo.getNombreUsuario() << " no tiene albumes disponibles para ti\n";
    } else {
        cout << "\nAlbumes de " << amigo.getNombreUsuario() << ":\n";
        for (int k = 0; k < visibles.size(); k++) {
            iteraciones++;
            cout << (k + 1) << ".- ";
            amigo.getAlbum(visibles[k]).mostrar();
        }
        cout << "Numero de album (0 para volver): ";
        n = leerEntero();
        if (n >= 1 && n <= visibles.size())
            memoriaCarrusel = carrusel(amigo.getAlbum(visibles[n - 1]));
    }
    reportarRecursos(sizeof(nombre) + sizeof(a) + sizeof(n) + sizeof(memoriaCarrusel)
                     + visibles.memoriaContenedor() + memoriaCarrusel);


bool UdeABook::guardarAmistades(const char* ruta) const {
    ofstream out(ruta);
    if (!out.is_open()) return false;
    for (int i = 0; i < usuarios.size(); i++) {
        const Usuario& u = usuarios[i];
        if (u.cantidadAmigos() == 0) continue;
        out << '*' << u.getNombreUsuario() << ':';
        for (int k = 0; k < u.cantidadAmigos(); k++) {
            if (k > 0) out << ',';
            out << usuarios[u.getAmigo(k)].getNombreUsuario();
        }
        out << ";\n";
    }
    out.flush();
    return out.good();
}

bool UdeABook::guardarSolicitudes(const char* ruta) const {
    ofstream out(ruta);
    if (!out.is_open()) return false;
    for (int r = 0; r < usuarios.size(); r++) {
        const Usuario& u = usuarios[r];
        for (int k = 0; k < u.cantidadSolicitudes(); k++)
            out << usuarios[u.getSolicitud(k)].getNombreUsuario() << ','
                << u.getNombreUsuario() << ";\n";
    }
    out.flush();
    return out.good();
}

bool UdeABook::guardarMensajes(const char* ruta) const {
    ofstream out(ruta);
    if (!out.is_open()) return false;
    for (int i = 0; i < mensajes.size(); i++) {
        const Mensaje& m = mensajes[i];
        out << '*' << usuarios[m.getEmisor()].getNombreUsuario() << ','
            << usuarios[m.getReceptor()].getNombreUsuario() << ','
            << m.getFecha() << ',' << (m.estaLeido() ? 1 : 0) << ";\n"
            << '{' << m.getTexto() << "}\n";
    }
    out.flush();
    return out.good();
}

bool UdeABook::guardarDatos() const {
    return guardarAmistades("amistades.txt")
    && guardarSolicitudes("solicitudes.txt")
        && guardarMensajes("mensajes.txt");
}

void UdeABook::guardarPendientes() {
    if (cambiosPendientes == 0) return;
    if (guardarDatos()) {
        cambiosPendientes = 0;
        cout << "[Datos guardados]\n";
    } else {
        cout << "Aviso: no se pudieron guardar los datos\n";  // se reintenta en el proximo cambio
    }
}