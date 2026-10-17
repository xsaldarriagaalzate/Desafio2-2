#ifndef FECHA_H
#define FECHA_H

#include <iostream>

using namespace std;

class Fecha {
private:
    int dia;
    int mes;
    int anio;

    static bool esBisiesto(int a);
    static int diasEnMes(int m, int a);

public:
    Fecha();
    Fecha(int dia, int mes, int anio);

    int getDia() const;
    int getMes() const;
    int getAnio() const;
    void setFecha(int d, int m, int a);

    bool esValida() const;

    long clave() const;

    bool desdeTexto(const char* texto);

    static Fecha hoy();

    bool operator==(const Fecha& o) const;
    bool operator!=(const Fecha& o) const;
    bool operator<(const Fecha& o) const;
    bool operator<=(const Fecha& o) const;
    bool operator>(const Fecha& o) const;
    bool operator>=(const Fecha& o) const;

    friend ostream& operator<<(ostream& os, const Fecha& f);
};

#endif // FECHA_H