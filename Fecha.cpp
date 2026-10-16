#include "Fecha.h"

Fecha::Fecha() : dia(1), mes(1), anio(2000) {}

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {}

bool Fecha::esBisiesto(int a) {
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

int Fecha::diasEnMes(int m, int a) {
    switch (m) {
    case 4: case 6: case 9: case 11: return 30;
    case 2: return esBisiesto(a) ? 29 : 28;
    default: return 31;
    }
}

int Fecha::getDia() const  { return dia; }
int Fecha::getMes() const  { return mes; }
int Fecha::getAnio() const { return anio; }

void Fecha::setFecha(int d, int m, int a) { dia = d; mes = m; anio = a; }

bool Fecha::esValida() const {
    if (anio < 1 || mes < 1 || mes > 12) return false;
    return dia >= 1 && dia <= diasEnMes(mes, anio);
}

long Fecha::clave() const {
    return (long)anio * 10000L + mes * 100L + dia;
}

bool Fecha::desdeTexto(const char* t) {
    if (!t) return false;
    int partes[3] = {0, 0, 0};
    int idx = 0;
    int digitos = 0;
    for (int i = 0; t[i] != '\0'; i++) {
        if (t[i] >= '0' && t[i] <= '9') {
            partes[idx] = partes[idx] * 10 + (t[i] - '0');
            digitos++;
        } else if (t[i] == '/' && digitos > 0 && idx < 2) {
            idx++;
            digitos = 0;
        } else {
            return false;
        }
    }
    if (idx != 2 || digitos == 0) return false;

    Fecha candidata(partes[0], partes[1], partes[2]);
    if (!candidata.esValida()) return false;
    *this = candidata;
    return true;
}

unsigned long Fecha::memoria() { return sizeof(Fecha); }

bool Fecha::operator==(const Fecha& o) const { return clave() == o.clave(); }
bool Fecha::operator!=(const Fecha& o) const { return clave() != o.clave(); }
bool Fecha::operator<(const Fecha& o) const  { return clave() <  o.clave(); }
bool Fecha::operator<=(const Fecha& o) const { return clave() <= o.clave(); }
bool Fecha::operator>(const Fecha& o) const  { return clave() >  o.clave(); }
bool Fecha::operator>=(const Fecha& o) const { return clave() >= o.clave(); }

std::ostream& operator<<(std::ostream& os, const Fecha& f) {
    if (f.dia < 10) os << '0';
    os << f.dia << '/';
    if (f.mes < 10) os << '0';
    os << f.mes << '/' << f.anio;
    return os;
}