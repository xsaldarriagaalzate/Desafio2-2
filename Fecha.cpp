#include "Fecha.h"
#include <chrono>

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
    if (anio < 1000 || anio > 9999 || mes < 1 || mes > 12) return false;
    return dia >= 1 && dia <= diasEnMes(mes, anio);
}

long Fecha::clave() const {
    return (long)(anio * 10000L) + (mes * 100L) + dia;
}

Fecha Fecha::hoy() {
    const long long DESFASE = -5LL * 3600;

    long long segundos = chrono::duration_cast<std::chrono::seconds>(
                             chrono::system_clock::now().time_since_epoch()).count();

    long long dias = (segundos + DESFASE) / 86400;   // días completos desde el 01/01/1970

    // Conversión de "días desde 1970" a día/mes/año (calendario gregoriano).
    long long z = dias + 719468;                     // días desde el 01/03/0000
    long long era = z / 146097;                      // bloques de 400 años
    long long doe = z - era * 146097;                // día dentro del bloque [0, 146096]
    long long yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;  // año dentro del bloque
    long long y = yoe + era * 400;
    long long doy = doe - (365 * yoe + yoe / 4 - yoe / 100);                // día dentro del año (desde marzo)
    long long mp = (5 * doy + 2) / 153;                                     // mes (0 = marzo)
    int d = (int)(doy - (153 * mp + 2) / 5 + 1);
    int m = (int)(mp < 10 ? mp + 3 : mp - 9);
    if (m <= 2) y++;                                 // enero y febrero pertenecen al año siguiente

    return Fecha(d, m, (int)y);
}

bool Fecha::desdeTexto(const char* texto) {
    if (!texto) return false;
    int partes[3] = {0, 0, 0};
    int idx = 0;
    int digitos = 0;
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] >= '0' && texto[i] <= '9') {
            partes[idx] = partes[idx] * 10 + (texto[i] - '0');
            digitos++;
            if (digitos > 4) return false;
        } else if (texto[i] == '/' && digitos > 0 && idx < 2) {
            idx++;
            digitos = 0;
        } else {
            return false;
        }
    }
    if (idx != 2 || digitos != 4) return false;

    Fecha candidata(partes[0], partes[1], partes[2]);
    if (!candidata.esValida()) return false;         // esValida también rechaza años < 1000 (como 0026)
    *this = candidata;
    return true;
}

void Fecha::aTexto(char* destino) const {
    destino[0]  = static_cast<char>('0' + dia / 10);
    destino[1]  = static_cast<char>('0' + dia % 10);
    destino[2]  = '/';
    destino[3]  = static_cast<char>('0' + mes / 10);
    destino[4]  = static_cast<char>('0' + mes % 10);
    destino[5]  = '/';
    destino[6]  = static_cast<char>('0' + anio / 1000);
    destino[7]  = static_cast<char>('0' + (anio / 100) % 10);
    destino[8]  = static_cast<char>('0' + (anio / 10) % 10);
    destino[9]  = static_cast<char>('0' + anio % 10);
    destino[10] = '\0';
}

Cadena Fecha::aCadena() const {
    char texto[11];
    aTexto(texto);
    return Cadena(texto);
}

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