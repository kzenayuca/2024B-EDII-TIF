#include "code.h"

int iteraciones = 100;  // Número de valores en la llave
double x0 = 0.5;        // Condición inicial (debe estar entre 0 y 1)
double r = 3.99;        // Parámetro de control (debe estar entre 3.57 y 4 para el caos)

std::vector<double> generarLlave() {
    std::vector<double> llave;
    double xn = x0;

    for (int i = 0; i < iteraciones; ++i) {
        xn = r * xn * (1 - xn);
        llave.push_back(xn);
    }

    return llave;
}