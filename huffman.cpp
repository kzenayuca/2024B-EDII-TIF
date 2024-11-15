#include "code.h"

// Construye el árbol de Huffman
Nodo* construirArbolHuffman(const std::string &texto) {
    // Cuenta las frecuencias de cada carácter
    std::unordered_map<char, int> frecuencia;
    for (char c : texto) {
        frecuencia[c]++;
    }

    // Crea una cola de prioridad con los nodos
    std::priority_queue<Nodo*, std::vector<Nodo*>, Comparador> heap;
    for (auto &par : frecuencia) {
        heap.push(new Nodo(par.first, par.second));
    }

    // Combina los nodos para construir el árbol
    while (heap.size() > 1) {
        Nodo *izq = heap.top(); heap.pop();
        Nodo *der = heap.top(); heap.pop();

        Nodo *nuevoNodo = new Nodo('\0', izq->frecuencia + der->frecuencia);
        nuevoNodo->izquierda = izq;
        nuevoNodo->derecha = der;
        heap.push(nuevoNodo);
    }

    return heap.top();
}

// Genera el código de Huffman para cada carácter
void generarCodigoHuffman(Nodo* raiz, std::string codigo, std::unordered_map<char, std::string> &tabla) {
    if (!raiz) return;

    if (!raiz->izquierda && !raiz->derecha) {
        tabla[raiz->caracter] = codigo;
    }

    generarCodigoHuffman(raiz->izquierda, codigo + "0", tabla);
    generarCodigoHuffman(raiz->derecha, codigo + "1", tabla);
}

// Codifica un texto usando la tabla de Huffman
std::string codificar(const std::string &texto, const std::unordered_map<char, std::string> &tabla) {
    std::string codigo;
    for (char c : texto) {
        codigo += tabla.at(c);
    }
    return codigo;
}

// Decodifica un texto usando el árbol de Huffman
std::string decodificar(const std::string &codigo, Nodo* raiz) {
    std::string texto;
    Nodo* actual = raiz;
    for (char bit : codigo) {
        actual = (bit == '0') ? actual->izquierda : actual->derecha;

        if (!actual->izquierda && !actual->derecha) {
            texto += actual->caracter;
            actual = raiz;
        }
    }
    return texto;
}