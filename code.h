#ifndef CODE_H_
#define CODE_H_

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>
#include <iomanip>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <stdexcept>
#include <memory>
#include <opencv2/opencv.hpp>
#include <bitset>
#include <stdexcept>

using namespace std;

// Nodo para el árbol de Huffman
struct Nodo {
    char caracter;
    int frecuencia;
    Nodo *izquierda, *derecha;

    Nodo(char c, int f) : caracter(c), frecuencia(f), izquierda(nullptr), derecha(nullptr) {}
};

// Comparador para ordenar el heap de prioridad
struct Comparador {
    bool operator()(Nodo* a, Nodo* b) {
        return a->frecuencia > b->frecuencia;
    }
};

Nodo* construirArbolHuffman(const std::string &texto);
void generarCodigoHuffman(Nodo* raiz, std::string codigo, std::unordered_map<char, std::string> &tabla);
std::string codificar(const std::string &texto, const std::unordered_map<char, std::string> &tabla);
std::string decodificar(const std::string &codigo, Nodo* raiz);

//MAPAS CAOTICOS
std::vector<double> generarLlave();

//AES

class CifradorAES {
private:
    static constexpr size_t TAMANO_LLAVE = 32; // Para AES-256
    static constexpr size_t TAMANO_IV = 16;
    static constexpr size_t TAMANO_TAG = 16;  // Para GCM
    
    std::vector<unsigned char> llave;

    void manejarError();
    public:
    std::vector<unsigned char> convertirLlave(const std::vector<double>& llaveEntrada);
    
    struct ResultadoCifrado {
        std::vector<unsigned char> iv;
        std::vector<unsigned char> tag;
        std::vector<unsigned char> datos;
    };
    ResultadoCifrado cifrar(const std::string& mensaje, const std::vector<double>& llaveEntrada);

};

std::string bytesAHex(const std::vector<unsigned char>& bytes);


///LSB
class EsteganografiaLSB {
private:
    std::vector<bool> bytesABits(const std::vector<unsigned char>& bytes);
    std::vector<unsigned char> bitsABytes(const std::vector<bool>& bits);
public:
    void ocultarDatos(const std::string& rutaImagen, 
                      const std::vector<unsigned char>& datosCifrados,
                      const std::vector<unsigned char>& iv,
                      const std::vector<unsigned char>& tag,
                      const std::string& rutaSalida);
    std::tuple<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>> 
    extraerDatos(const std::string& rutaImagen);
};

#endif