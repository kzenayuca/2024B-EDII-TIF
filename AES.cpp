#include "code.h"

    void CifradorAES::manejarError() {
        char buf[256];
        ERR_error_string_n(ERR_get_error(), buf, sizeof(buf));
        throw std::runtime_error(buf);
    }
    //0 - 255
    std::vector<unsigned char> CifradorAES::convertirLlave(const std::vector<double>& llaveEntrada) {
        std::vector<unsigned char> llaveAES(TAMANO_LLAVE, 0);
        for (size_t i = 0; i < llaveEntrada.size() && i < llaveAES.size(); ++i) {
            llaveAES[i] = static_cast<unsigned char>(llaveEntrada[i] * 255);
        }
        return llaveAES;
    }

    

    CifradorAES::ResultadoCifrado CifradorAES::cifrar(const std::string& mensaje, const std::vector<double>& llaveEntrada) {
        // Inicializar OpenSSL
        EVP_add_cipher(EVP_aes_256_gcm());
        
        // Convertir y validar la llave
        llave = convertirLlave(llaveEntrada);

       // for(auto &i: llave){
      //      cout<<i<<"-";
       // }
       // cout<<endl;
        if (llave.size() != TAMANO_LLAVE) {
            throw std::runtime_error("Tamaño de llave inválido");
        }

        // Generar IV aleatorio
        std::vector<unsigned char> iv(TAMANO_IV);
        if (RAND_bytes(iv.data(), TAMANO_IV) != 1) {
            manejarError();
        }

        // Preparar el contexto de cifrado
        auto ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            manejarError();
        }
        
        // Configurar el cifrado
        if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, llave.data(), iv.data())) {
            EVP_CIPHER_CTX_free(ctx);
            manejarError();
        }

        // Preparar el buffer de salida
        std::vector<unsigned char> datosCifrados(mensaje.size() + EVP_MAX_BLOCK_LENGTH);
        int longitudEscrita = 0;
        int longitudTotal = 0;

        // Cifrar los datos
        if (!EVP_EncryptUpdate(ctx, datosCifrados.data(), &longitudEscrita,
                              reinterpret_cast<const unsigned char*>(mensaje.data()),
                              mensaje.size())) {
            EVP_CIPHER_CTX_free(ctx);
            manejarError();
        }
        longitudTotal = longitudEscrita;

        // Finalizar el cifrado
        if (!EVP_EncryptFinal_ex(ctx, datosCifrados.data() + longitudEscrita, &longitudEscrita)) {
            EVP_CIPHER_CTX_free(ctx);
            manejarError();
        }
        longitudTotal += longitudEscrita;

        // Obtener el tag de autenticación
        std::vector<unsigned char> tag(TAMANO_TAG);
        if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, TAMANO_TAG, tag.data())) {
            EVP_CIPHER_CTX_free(ctx);
            manejarError();
        }

        // Limpiar el contexto
        EVP_CIPHER_CTX_free(ctx);

        // Ajustar el tamaño del vector de datos cifrados
        datosCifrados.resize(longitudTotal);

        return {iv, tag, datosCifrados};
    }

std::string CifradorAES::descifrarAES(const CifradorAES::ResultadoCifrado& resultado, const std::vector<double>& llaveEntrada) {
    // Inicializar OpenSSL
    EVP_add_cipher(EVP_aes_256_gcm());

    // Convertir y validar la llave
    llave = convertirLlave(llaveEntrada);
    if (llave.size() != TAMANO_LLAVE) {
        throw std::runtime_error("Tamaño de llave inválido");
    }

    // Preparar el contexto de descifrado
    auto ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        manejarError();
    }

    // Configurar el descifrado
    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, llave.data(), resultado.iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        manejarError();
    }

    // Preparar el buffer de salida
    std::vector<unsigned char> datosDescifrados(resultado.datos.size());
    int longitudEscrita = 0;
    int longitudTotal = 0;

    // Descifrar los datos
    if (!EVP_DecryptUpdate(ctx, datosDescifrados.data(), &longitudEscrita,
                           resultado.datos.data(), resultado.datos.size())) {
        EVP_CIPHER_CTX_free(ctx);
        manejarError();
    }
    longitudTotal = longitudEscrita;

    // Configurar el tag para la verificación de autenticidad
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, TAMANO_TAG, const_cast<unsigned char*>(resultado.tag.data()))) {
        EVP_CIPHER_CTX_free(ctx);
        manejarError();
    }

    // Finalizar el descifrado
    if (!EVP_DecryptFinal_ex(ctx, datosDescifrados.data() + longitudEscrita, &longitudEscrita)) {
        EVP_CIPHER_CTX_free(ctx);
        manejarError();
    }
    longitudTotal += longitudEscrita;

    // Limpiar el contexto
    EVP_CIPHER_CTX_free(ctx);

    // Ajustar el tamaño del vector de datos descifrados
    datosDescifrados.resize(longitudTotal);

    // Convertir los datos descifrados a cadena de texto
    return std::string(datosDescifrados.begin(), datosDescifrados.end());
}

// Función auxiliar para imprimir bytes en hexadecimal
std::string bytesAHex(const std::vector<unsigned char>& bytes) {
    std::string resultado;
    for (unsigned char byte : bytes) {
        char buf[3];
        snprintf(buf, sizeof(buf), "%02x", byte);
        resultado += buf;
    }
    return resultado;
}
