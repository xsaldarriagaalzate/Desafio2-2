#include "Mensaje.h"

using namespace std;

static int largoHasta(const char* t, int tope) {
    int n = 0;
    if (t) {
        while (t[n] != '\0' && n < tope) n++;
    }
    return n;
}

Mensaje::Mensaje() : emisor(-1), receptor(-1), leido(false) {}

Mensaje::Mensaje(int e, int r, const char* t, const Fecha& f, bool l) {
    emisor = e;
    receptor = r;
    int n = largoHasta(t, MAX_TEXTO);
    texto = Cadena(t ? t : "", n);
    fecha = f;
    leido = l;
}

Mensaje::Mensaje(int e, int r, const char* t)
    : Mensaje(e, r, t, Fecha::hoy(), false) {}

bool Mensaje::textoValido(const char* t) {
    int n = largoHasta(t, MAX_TEXTO + 1);
    return n > 0 && n <= MAX_TEXTO;
}

int Mensaje::getEmisor() const { return emisor; }
int Mensaje::getReceptor() const { return receptor; }
const char* Mensaje::getTexto() const { return texto.cadena_char(); }
const Fecha& Mensaje::getFecha() const { return fecha; }
bool Mensaje::estaLeido() const { return leido; }

void Mensaje::marcarLeido() { leido = true; }
void Mensaje::setLeido(bool valor) { leido = valor; }

void Mensaje::mostrar() const {
    cout << *this << "\n";
}

void Mensaje::mostrarCon(const char* nombreEmisor, const char* nombreReceptor) const {
    cout << "[" << fecha << "] " << nombreEmisor << " -> " << nombreReceptor
              << (leido ? " (leido): " : " (no leido): ") << texto.cadena_char() << "\n";
}

bool Mensaje::operator<(const Mensaje& otro) const { return fecha < otro.fecha; }

bool Mensaje::operator==(const Mensaje& otro) const {
    return emisor == otro.emisor && receptor == otro.receptor &&
           fecha == otro.fecha && leido == otro.leido && texto == otro.texto;
}

ostream& operator<<(ostream& os, const Mensaje& m) {
    os << "[" << m.fecha << "] #" << m.emisor << " -> #" << m.receptor
       << (m.leido ? " (leido): " : " (no leido): ") << m.texto.cadena_char();
    return os;
}