#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

// Función para calcular el Error Cuadrático Medio (MSE)
double calculateMSE(const Mat& original, const Mat& processed) {
    if (original.size() != processed.size() || original.type() != processed.type()) {
        cerr << "Error: Las imágenes deben tener el mismo tamaño y tipo." << endl;
        return -1;
    }

    double mse = 0.0;
    int totalPixels = original.rows * original.cols * original.channels();

    for (int i = 0; i < original.rows; i++) {
        for (int j = 0; j < original.cols; j++) {
            Vec3b pixelOriginal = original.at<Vec3b>(i, j);
            Vec3b pixelProcessed = processed.at<Vec3b>(i, j);

            for (int c = 0; c < original.channels(); c++) {
                double diff = static_cast<double>(pixelOriginal[c]) - static_cast<double>(pixelProcessed[c]);
                mse += diff * diff;
            }
        }
    }

    mse /= totalPixels;
    return mse;
}

// Función para calcular el PSNR
double calculatePSNR(const Mat& original, const Mat& processed) {
    double mse = calculateMSE(original, processed);
    if (mse == 0) {
        return INFINITY; // PSNR infinito si no hay diferencia entre las imágenes
    }

    double maxPixelValue = 255.0; // Máximo valor de un píxel para imágenes de 8 bits
    double psnr = 10.0 * log10((maxPixelValue * maxPixelValue) / mse);
    return psnr;
}

int main() {
    // Carga de las imágenes
    Mat original = imread("Images/Monkey.jpg");
    Mat processed = imread("Images/Monkey.jpg");

    if (original.empty() || processed.empty()) {
        cerr << "Error al cargar las imágenes. Verifica las rutas." << endl;
        return -1;
    }

    // Cálculo del PSNR
    double psnr = calculatePSNR(original, processed);

    if (psnr == -1) {
        cerr << "Error al calcular el PSNR." << endl;
    } else {
        cout << "El valor de PSNR es: " << psnr << " dB" << endl;
    }

    return 0;
}
