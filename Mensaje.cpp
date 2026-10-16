#include "Mensaje.h"

int Mensaje::longitud(const char* s) {
    int n = 0;
    if (s) while (s[n] != '\0') n++;
    return n;
}

char* Mensaje::duplicar(const char* s) {
    int n = longitud(s);
    char* copia = new char[n + 1];
    for (int i = 0; i < n; i++) copia[i] = s[i];
    copia[n] = '\0';
    return copia;
}

Mensaje::Mensaje()
    : emisor(duplicar("")), receptor(duplicar("")), texto(duplicar("")),
    fecha(), leido(false) {}

Mensaje::Mensaje(const char* e, const char* r, const char* t,
                 const Fecha& f, bool l)
    : emisor(duplicar(e)), receptor(duplicar(r)), texto(nullptr),
    fecha(f), leido(l) {
    int n = longitud(t);
    if (n > MAX_TEXTO) n = MAX_TEXTO;
    texto = new char[n + 1];
    for (int i = 0; i < n; i++) texto[i] = t[i];
    texto[n] = '\0';
}

Mensaje::Mensaje(const Mensaje& o)
    : emisor(duplicar(o.emisor)), receptor(duplicar(o.receptor)),
    texto(duplicar(o.texto)), fecha(o.fecha), leido(o.leido) {}

Mensaje& Mensaje::operator=(const Mensaje& o) {
    if (this == &o) return *this;
    char* nuevoE = duplicar(o.emisor);
    char* nuevoR = duplicar(o.receptor);
    char* nuevoT = duplicar(o.texto);
    delete[] emisor;
    delete[] receptor;
    delete[] texto;
    emisor = nuevoE;
    receptor = nuevoR;
    texto = nuevoT;
    fecha = o.fecha;
    leido = o.leido;
    return *this;
}

Mensaje::~Mensaje() {
    delete[] emisor;
    delete[] receptor;
    delete[] texto;
}

bool Mensaje::textoValido(const char* t) {
    int n = longitud(t);
    return n > 0 && n <= MAX_TEXTO;
}

const char* Mensaje::getEmisor() const   { return emisor; }
const char* Mensaje::getReceptor() const { return receptor; }
const char* Mensaje::getTexto() const    { return texto; }
const Fecha& Mensaje::getFecha() const   { return fecha; }
bool Mensaje::estaLeido() const          { return leido; }

void Mensaje::marcarLeido()        { leido = true; }
void Mensaje::setLeido(bool valor) { leido = valor; }

unsigned long Mensaje::memoria() const {
    return sizeof(*this)
    + (longitud(emisor) + 1)
        + (longitud(receptor) + 1)
        + (longitud(texto) + 1);
}

void Mensaje::mostrar() const {
    std::cout << *this << "\n";
}

bool Mensaje::operator<(const Mensaje& o) const  { return fecha < o.fecha; }
bool Mensaje::operator==(const Mensaje& o) const {
    if (!(fecha == o.fecha) || leido != o.leido) return false;
    int a = longitud(texto), b = longitud(o.texto);
    if (a != b) return false;
    for (int i = 0; i < a; i++) if (texto[i] != o.texto[i]) return false;
    return true;
}

std::ostream& operator<<(std::ostream& os, const Mensaje& m) {
    os << "[" << m.fecha << "] " << m.emisor << " -> " << m.receptor
       << (m.leido ? " (leido): " : " (no leido): ") << m.texto;
    return os;
}