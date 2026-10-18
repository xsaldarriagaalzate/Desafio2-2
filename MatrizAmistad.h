#ifndef MATRIZAMISTAD_H
#define MATRIZAMISTAD_H

#include <cstddef>   // size_t

// Matriz de adyacencia de las amistades: celda (i, j) vale 1 si el usuario i y el usuario j son amigos.
// Es simétrica (la amistad es mutua) y la diagonal siempre es 0.
// Se guarda en UN solo bloque de n*n bytes: celdas[i * n + j].
class MatrizAmistad {
private:
    char* celdas;
    int n;

public:
    // Regla de los tres: la clase tiene un puntero con memoria dinámica
    MatrizAmistad();
    MatrizAmistad(const MatrizAmistad& otra);
    MatrizAmistad& operator=(const MatrizAmistad& otra);
    ~MatrizAmistad();

    void redimensionar(int nuevoN);                  // descarta lo anterior y deja todo en 0
    int tamano() const { return n; }

    void agregarAmistad(int i, int j);               // marca (i,j) y (j,i)
    bool sonAmigos(int i, int j) const;

    // Cantidad de amigos que i y j tienen en común. Suma 1 a 'iteraciones' por cada celda revisada.
    int amigosEnComun(int i, int j, long& iteraciones) const;

    size_t memoriaDinamica() const;
    size_t memoria() const;                          // el objeto + las n*n celdas del heap
};

#endif // MATRIZAMISTAD_H
