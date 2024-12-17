#include "code.h"


int main() {
    string texto = "Hola Mundo!";
    //cout<<texto<<endl;
    //cout<<endl;

    Nodo* raiz = construirArbolHuffman(texto);

    unordered_map<char, std::string> tabla;
    generarCodigoHuffman(raiz, "", tabla);

    string textoCodificado = codificar(texto, tabla);
    string textoDecodificado = decodificar(textoCodificado,raiz);

    //cout << "\nTexto codificado con Huffman:\n" << textoCodificado << '\n';
    //cout << "\nTexto decodificado: \n" << textoDecodificado << '\n';
    //cout<<endl;

    std::vector<double> llave = generarLlave(); //vector 100 datos
/*
    std::cout << "\nLlave generada con el mapa caótico logístico:" << std::endl;

    for (double valor : llave) {
        std::cout << std::fixed << std::setprecision(8) << valor << " ";
    }
*/
    //cout<<endl;

    /////////////////////////////////////////////////////////////////////////
    try {

        CifradorAES cifrador;
        auto resultadoCifrado = cifrador.cifrar(textoCodificado, llave); //metiendo un string, vector de 100 datos

        //std::cout << "\nIV: " << bytesAHex(resultadoCifrado.iv) << std::endl;
        //std::cout << "Tag: " << bytesAHex(resultadoCifrado.tag) << std::endl;
        //std::cout << "Datos cifrados con AES: " << bytesAHex(resultadoCifrado.datos) << std::endl;
        //string desAES = cifrador.descifrarAES(resultadoCifrado,llave);
        //cout<<"Datos Descifrados con AES: "<<desAES<<endl;

    //////////////////////////////////////////////////////////////////////////
    std::string imagePath = "Images/Monkey.jpg";
    std::string outputImagePath = "imagen_con_mensaje2.png";
    //std::string message = "El modernismo catalán fue un movimiento cultural que se dio en Cataluña entre finales del siglo XIX y principios del xx, que se plasmó tanto en el arte (arquitectura, pintura, escultura y otras disciplinas artísticas) como en la literatura, la música y diversas manifestaciones culturales. Fue la culminación de un proceso de renacimiento cultural —la Renaixença— que supuso tanto la revalorización de la lengua catalana como un período de esplendor en todos los campos de la cultura —uno de los pocos en los que fue equiparable a cualquiera de los principales focos artísticos del momento en el continente europeo— acompañado por un período de bienestar económico patrocinado por el auge de la burguesía catalana y su gran desarrollo industrial, comercial y financiero.";
    std::string message = bytesAHex(resultadoCifrado.datos);
    // Ocultar mensaje
    hideMessage(imagePath, outputImagePath, message);

    // Extraer mensaje
    std::string binaryMessage = extractMessage(outputImagePath);
    std::string extractedMessage = binaryToText(binaryMessage);

    std::cout<<"Mensaje Insertado Exitosamente."<<std::endl;
    std::cout << "Mensaje extraído: " << extractedMessage << std::endl;


    string desAES = cifrador.descifrarAES(resultadoCifrado,llave);
    cout<<"Datos Descifrados con AES: "<<desAES<<endl;
    string txtDeco = decodificar(desAES,raiz);
    cout<<"Mensaje Descifrado: "<<txtDeco<<endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}
