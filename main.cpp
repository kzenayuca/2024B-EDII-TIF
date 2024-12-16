#include "code.h"

int main() {
    string texto = "Prueba con un mensaje secreto";
    //string texto = "El modernismo catalán fue un movimiento cultural que se dio en Cataluña entre finales del siglo XIX y principios del xx, que se plasmó tanto en el arte (arquitectura, pintura, escultura y otras disciplinas artísticas) como en la literatura, la música y diversas manifestaciones culturales. Fue la culminación de un proceso de renacimiento cultural —la Renaixença— que supuso tanto la revalorización de la lengua catalana como un período de esplendor en todos los campos de la cultura —uno de los pocos en los que fue equiparable a cualquiera de los principales focos artísticos del momento en el continente europeo— acompañado por un período de bienestar económico patrocinado por el auge de la burguesía catalana y su gran desarrollo industrial, comercial y financiero. ";

    for (char n : texto) {
        cout << n;
    }

    cout<<endl;

    Nodo* raiz = construirArbolHuffman(texto);

    unordered_map<char, std::string> tabla;
    generarCodigoHuffman(raiz, "", tabla);

    string textoCodificado = codificar(texto, tabla);
    cout << "\nTexto codificado con Huffman:\n" << textoCodificado << '\n';
    cout<<endl;

    //string textoDecodificado = decodificar(textoCodificado, raiz);
    //cout << "\nTexto decodificado:\n" << textoDecodificado << '\n';
    /////////////////////////////////////////////////////////////////

    std::vector<double> llave = generarLlave();

    std::cout << "\nLlave generada con el mapa caótico logístico:" << std::endl;
    for (double valor : llave) {
        std::cout << std::fixed << std::setprecision(8) << valor << " ";
    }

    cout<<endl;

    /////////////////////////////////////////////////////////////////////////
    try {

        CifradorAES cifrador;
        auto resultadoCifrado = cifrador.cifrar(textoCodificado, llave);

        std::cout << "\nIV: " << bytesAHex(resultadoCifrado.iv) << std::endl;
        std::cout << "Tag: " << bytesAHex(resultadoCifrado.tag) << std::endl;
        std::cout << "Datos cifrados con AES: " << bytesAHex(resultadoCifrado.datos) << std::endl;

    //////////////////////////////////////////////////////////////////////////

        EsteganografiaLSB estego;

        // Ocultar datos en la imagen
        estego.ocultarDatos(
            "Images/jellyfish.jpg",           // Ruta de la imagen original
            resultadoCifrado.datos,          // Datos cifrados
            resultadoCifrado.iv,             // Vector de inicialización
            resultadoCifrado.tag,            // Tag de autenticación
            "imagen_con_mensaje4.png"         // Ruta donde se guardará la imagen con el mensaje
        );

        std::cout << "\nMensaje ocultado exitosamente en la imagen." << std::endl;

        // Extraer datos de la imagen
        auto [ivExtraido, tagExtraido, datosCifradosExtraidos] = 
            estego.extraerDatos("imagen_con_mensaje4.png");

        std::cout << "Datos extraídos exitosamente." << std::endl;

        // Verificar que los datos extraídos coinciden con los originales
        bool datosCorrectos = 
            (ivExtraido == resultadoCifrado.iv) &&
            (tagExtraido == resultadoCifrado.tag) &&
            (datosCifradosExtraidos == resultadoCifrado.datos);

        std::cout << "Verificación de datos: " 
                  << (datosCorrectos ? "Exitosa" : "Fallida") << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
