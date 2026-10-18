#ifndef LISTA_H
#define LISTA_H

#include <cstddef>   // size_t

template <typename T>

class Lista {

private:
    T* datos;
    int tam;
    int cap;

    void crecer() {
        cap *= 2;
        T* nuevo = new T[cap];
        for (int i = 0; i < tam; i++) {
            nuevo[i] = datos[i];
        }
        delete[] datos;
        datos = nuevo;
    }

public:

    Lista() : datos(new T[4]), tam(0), cap(4) {}

    Lista(const Lista& otra) : datos(new T[otra.cap]), tam(otra.tam), cap(otra.cap) {
        for (int i = 0; i < tam; i++) datos[i] = otra.datos[i];
    }

    Lista& operator=(const Lista& otra) {
        if (this == &otra) return *this;   // evita autoasignación (a = a)
        delete[] datos;
        cap = otra.cap;
        tam = otra.tam;
        datos = new T[cap];
        for (int i = 0; i < tam; i++) datos[i] = otra.datos[i];
        return *this;
    }

    ~Lista() { delete[] datos; }

    void agregar(const T& x) {
        if (tam == cap) crecer();
        datos[tam++] = x;
    }

    T& operator[](int i) { return datos[i]; }
    const T& operator[](int i) const { return datos[i]; }

    void quitarEn(int i) {
        if (i < 0 || i >= tam) return;
        for (int k = i; k + 1 < tam; k++) datos[k] = datos[k + 1];
        tam--;
        datos[tam] = T();
    }

    void eliminarEn(int pos) {
        for (int i = pos; i < tam - 1; i++) datos[i] = datos[i + 1];
        tam--;
    }

    int size() const { return tam; }
    int capacidad() const { return cap; }

    size_t memoriaArreglo() const { return (size_t)cap * sizeof(T); }
    size_t memoriaContenedor() const { return sizeof(*this) + memoriaArreglo(); }
};

#endif