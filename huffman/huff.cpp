#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <vector>

using namespace std;

// Estructura del nodo del árbol de Huffman
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// Comparador para la cola de prioridad (min heap)
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// Función para crear el árbol de Huffman
HuffmanNode* buildHuffmanTree(unordered_map<char, int>& freqMap) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> minHeap;

    // Crear un nodo para cada carácter y agregarlo al heap
    for (auto& pair : freqMap) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    // Iterar hasta que el heap tenga un solo nodo
    while (minHeap.size() != 1) {
        HuffmanNode* left = minHeap.top();
        minHeap.pop();
        HuffmanNode* right = minHeap.top();
        minHeap.pop();

        // Crear un nuevo nodo que combine los dos nodos más pequeños
        HuffmanNode* sum = new HuffmanNode('\0', left->freq + right->freq);
        sum->left = left;
        sum->right = right;
        minHeap.push(sum);
    }

    return minHeap.top();
}

// Función para generar los códigos de Huffman
void generateHuffmanCodes(HuffmanNode* root, string str, unordered_map<char, string>& huffmanCode) {
    if (!root)
        return;

    // Si es una hoja, guarda el código
    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateHuffmanCodes(root->left, str + "0", huffmanCode);
    generateHuffmanCodes(root->right, str + "1", huffmanCode);
}

// Función para calcular la frecuencia de los caracteres
unordered_map<char, int> calculateFrequencies(const string& text) {
    unordered_map<char, int> freqMap;
    for (char ch : text) {
        freqMap[ch]++;
    }
    return freqMap;
}

// Función principal para codificar el mensaje y escribir el resultado en un archivo
void encodeHuffman(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName);
    if (!inputFile.is_open()) {
        cerr << "No se pudo abrir el archivo de entrada.\n";
        return;
    }

    // Leer todo el contenido del archivo
    string text((istreambuf_iterator<char>(inputFile)), (istreambuf_iterator<char>()));
    inputFile.close();

    // Calcular la frecuencia de los caracteres
    unordered_map<char, int> freqMap = calculateFrequencies(text);

    // Construir el árbol de Huffman
    HuffmanNode* root = buildHuffmanTree(freqMap);

    // Generar los códigos de Huffman
    unordered_map<char, string> huffmanCode;
    generateHuffmanCodes(root, "", huffmanCode);

    // Escribir la codificación en el archivo de salida
    ofstream outputFile(outputFileName);
    if (!outputFile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida.\n";
        return;
    }

    // Guardar la codificación en el archivo
    for (char ch : text) {
        outputFile << huffmanCode[ch];
    }

    outputFile.close();
    cout << "Codificación Huffman guardada en: " << outputFileName << endl;
}

int main() {
    string inputFileName = "mensaje.txt";  // Nombre del archivo de entrada
    string outputFileName = "mensaje_codificado.txt";  // Nombre del archivo de salida

    encodeHuffman(inputFileName, outputFileName);

    return 0;
}
