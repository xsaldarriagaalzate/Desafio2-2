#ifndef REDAMISTADES_H
#define REDAMISTADES_H
#include <cstddef>

class RedAmistades {
private:
    int n;
    long posicion(int i, int j) const { return (long)i * n + j; }
    void poner(int i, int j);

public:
    RedAmistades();
    explicit RedAmistades(int n);
    RedAmistades(const RedAmistades& otra);
    RedAmistades& operator=(const RedAmistades& otra);
    ~RedAmistades();

    void conectar(int a, int b);
    bool sonAmigos(int a, int b) const;
    int amigosEnComun(int a, int b) const;
};
#endif