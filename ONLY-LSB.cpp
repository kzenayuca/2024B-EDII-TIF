#include <iostream>
#include <opencv2/opencv.hpp>
#include <bitset>
#include <string>

void hideMessage(const std::string& imagePath, const std::string& outputImagePath, const std::string& message) {
    // Cargar la imagen
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: No se pudo cargar la imagen." << std::endl;
        return;
    }

    // Convertir el mensaje a una cadena binaria
    std::string binaryMessage;
    for (char c : message) {
        binaryMessage += std::bitset<8>(c).to_string(); // Convertir cada carácter a 8 bits
    }
    binaryMessage += "00000000"; // Delimitador de fin de mensaje

    size_t messageIndex = 0;
    bool messageFits = true;

    // Insertar el mensaje en los bits menos significativos de los píxeles
    for (int row = 0; row < image.rows && messageFits; ++row) {
        for (int col = 0; col < image.cols && messageFits; ++col) {
            for (int channel = 0; channel < 3 && messageFits; ++channel) { // Iterar sobre B, G, R
                if (messageIndex < binaryMessage.size()) {
                    // Modificar el LSB del canal actual
                    uchar& pixel = image.at<cv::Vec3b>(row, col)[channel];
                    pixel = (pixel & 0xFE) | (binaryMessage[messageIndex] - '0');
                    ++messageIndex;
                } else {
                    messageFits = false;
                }
            }
        }
    }

    if (messageIndex < binaryMessage.size()) {
        std::cerr << "Error: El mensaje es demasiado grande para esta imagen." << std::endl;
        return;
    }

    // Guardar la imagen con el mensaje oculto
    cv::imwrite(outputImagePath, image);
    std::cout << "Mensaje ocultado exitosamente en " << outputImagePath << std::endl;
}

std::string extractMessage(const std::string& imagePath) {
    // Cargar la imagen
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        std::cerr << "Error: No se pudo cargar la imagen." << std::endl;
        return "";
    }

    std::string binaryMessage;
    char currentChar = 0;
    int bitCount = 0;

    // Extraer los bits menos significativos de los píxeles
    for (int row = 0; row < image.rows; ++row) {
        for (int col = 0; col < image.cols; ++col) {
            for (int channel = 0; channel < 3; ++channel) { // Iterar sobre B, G, R
                uchar pixel = image.at<cv::Vec3b>(row, col)[channel];
                binaryMessage += (pixel & 1) ? '1' : '0';
                ++bitCount;

                // Convertir cada 8 bits en un carácter
                if (bitCount == 8) {
                    currentChar = std::bitset<8>(binaryMessage.substr(binaryMessage.size() - 8)).to_ulong();
                    if (currentChar == '\0') {
                        return binaryMessage.substr(0, binaryMessage.size() - 8);
                    }
                    bitCount = 0;
                }
            }
        }
    }

    std::cerr << "Error: No se encontró un delimitador de fin de mensaje." << std::endl;
    return "";
}

std::string binaryToText(const std::string& binaryMessage) {
    std::string text;
    for (size_t i = 0; i < binaryMessage.size(); i += 8) {
        std::bitset<8> byte(binaryMessage.substr(i, 8));
        text += static_cast<char>(byte.to_ulong());
    }
    return text;
}

int main() {
    std::string imagePath = "Images/jellyfish.jpg";
    std::string outputImagePath = "imagen_modificada4.png";
    //std::string message = "El modernismo catalán fue un movimiento cultural que se dio en Cataluña entre finales del siglo XIX y principios del xx, que se plasmó tanto en el arte (arquitectura, pintura, escultura y otras disciplinas artísticas) como en la literatura, la música y diversas manifestaciones culturales. Fue la culminación de un proceso de renacimiento cultural —la Renaixença— que supuso tanto la revalorización de la lengua catalana como un período de esplendor en todos los campos de la cultura —uno de los pocos en los que fue equiparable a cualquiera de los principales focos artísticos del momento en el continente europeo— acompañado por un período de bienestar económico patrocinado por el auge de la burguesía catalana y su gran desarrollo industrial, comercial y financiero.";
    std::string message = "Prueba con un mensaje secreto";
    // Ocultar mensaje
    hideMessage(imagePath, outputImagePath, message);

    // Extraer mensaje
    std::string binaryMessage = extractMessage(outputImagePath);
    std::string extractedMessage = binaryToText(binaryMessage);

    std::cout << "Mensaje extraído: " << extractedMessage << std::endl;

    return 0;
}
