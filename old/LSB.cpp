#include "code.h"

    // Convierte un vector de bytes a un vector de bits
    std::vector<bool> EsteganografiaLSB::bytesABits(const std::vector<unsigned char>& bytes) {
        std::vector<bool> bits;
        for (unsigned char byte : bytes) {
            std::bitset<8> bitset(byte);
            for (int i = 0; i < 8; i++) {
                bits.push_back(bitset[i]);
            }
        }
        return bits;
    }

    // Convierte un vector de bits a bytes
    std::vector<unsigned char> EsteganografiaLSB::bitsABytes(const std::vector<bool>& bits) {
        std::vector<unsigned char> bytes;
        for (size_t i = 0; i < bits.size(); i += 8) {
            std::bitset<8> bitset;
            for (int j = 0; j < 8 && (i + j) < bits.size(); j++) {
                bitset[j] = bits[i + j];
            }
            bytes.push_back(static_cast<unsigned char>(bitset.to_ulong()));
        }
        return bytes;
    }


    void EsteganografiaLSB::ocultarDatos(const std::string& rutaImagen, 
                      const std::vector<unsigned char>& datosCifrados,
                      const std::vector<unsigned char>& iv,
                      const std::vector<unsigned char>& tag,
                      const std::string& rutaSalida) {
        // Cargar la imagen
        cv::Mat imagen = cv::imread(rutaImagen);
        if (imagen.empty()) {
            throw std::runtime_error("No se pudo cargar la imagen");
        }

        // Preparar los datos a ocultar
        // Formato: [longitud(4 bytes)][iv][tag][datos]
        std::vector<unsigned char> datosCompletos;
        
        // Agregar longitud de datos cifrados (4 bytes)
        uint32_t longitud = datosCifrados.size();
        datosCompletos.push_back((longitud >> 24) & 0xFF);
        datosCompletos.push_back((longitud >> 16) & 0xFF);
        datosCompletos.push_back((longitud >> 8) & 0xFF);
        datosCompletos.push_back(longitud & 0xFF);

        // Agregar IV, tag y datos cifrados
        datosCompletos.insert(datosCompletos.end(), iv.begin(), iv.end());
        datosCompletos.insert(datosCompletos.end(), tag.begin(), tag.end());
        datosCompletos.insert(datosCompletos.end(), datosCifrados.begin(), datosCifrados.end());

        // Convertir a bits
        std::vector<bool> bits = bytesABits(datosCompletos);

        // Verificar si hay suficiente espacio en la imagen
        if (bits.size() > imagen.total() * imagen.channels()) {
            throw std::runtime_error("La imagen es muy pequeña para los datos");
        }

        // Ocultar los bits en la imagen
        size_t bitIndex = 0;
        for (int i = 0; i < imagen.rows && bitIndex < bits.size(); i++) {
            for (int j = 0; j < imagen.cols && bitIndex < bits.size(); j++) {
                for (int k = 0; k < imagen.channels() && bitIndex < bits.size(); k++) {
                    // Modificar el bit menos significativo
                    uint8_t& pixel = imagen.at<cv::Vec3b>(i, j)[k];
                    pixel = (pixel & 0xFE) | bits[bitIndex];
                    bitIndex++;
                }
            }
        }

        // Guardar la imagen resultante
        cv::imwrite(rutaSalida, imagen);
    }

    std::tuple<std::vector<unsigned char>, std::vector<unsigned char>, std::vector<unsigned char>> 
    EsteganografiaLSB::extraerDatos(const std::string& rutaImagen) {
        // Cargar la imagen
        cv::Mat imagen = cv::imread(rutaImagen);
        if (imagen.empty()) {
            throw std::runtime_error("No se pudo cargar la imagen");
        }

        // Extraer bits
        std::vector<bool> bits;
        for (int i = 0; i < imagen.rows; i++) {
            for (int j = 0; j < imagen.cols; j++) {
                for (int k = 0; k < imagen.channels(); k++) {
                    bits.push_back(imagen.at<cv::Vec3b>(i, j)[k] & 1);
                }
            }
        }

        // Convertir los primeros 4 bytes a longitud
        std::vector<unsigned char> longitudBytes = bitsABytes(std::vector<bool>(bits.begin(), bits.begin() + 32));
        uint32_t longitud = (longitudBytes[0] << 24) | (longitudBytes[1] << 16) | 
                           (longitudBytes[2] << 8) | longitudBytes[3];

        // Calcular tamaños
        const size_t tamanoIV = 16;  // 16 bytes para IV
        const size_t tamanoTag = 16; // 16 bytes para Tag
        const size_t totalBits = (4 + tamanoIV + tamanoTag + longitud) * 8;

        if (totalBits > bits.size()) {
            throw std::runtime_error("Datos incompletos en la imagen");
        }

        // Extraer IV
        auto bitsIV = std::vector<bool>(bits.begin() + 32, bits.begin() + 32 + (tamanoIV * 8));
        std::vector<unsigned char> iv = bitsABytes(bitsIV);

        // Extraer Tag
        auto bitsTag = std::vector<bool>(bits.begin() + 32 + (tamanoIV * 8), 
                                       bits.begin() + 32 + (tamanoIV * 8) + (tamanoTag * 8));
        std::vector<unsigned char> tag = bitsABytes(bitsTag);

        // Extraer datos cifrados
        auto bitsDatos = std::vector<bool>(bits.begin() + 32 + (tamanoIV * 8) + (tamanoTag * 8),
                                         bits.begin() + 32 + (tamanoIV * 8) + (tamanoTag * 8) + (longitud * 8));
        std::vector<unsigned char> datosCifrados = bitsABytes(bitsDatos);

        return {iv, tag, datosCifrados};
    }
