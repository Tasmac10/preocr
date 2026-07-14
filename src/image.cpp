#include "image.hpp"

#include <fstream>
#include <stdexcept>
#include <cctype>


namespace preocr {

// [Membro 1] TODO: implementar leitura de PBM P1.
// Deve tratar: magic number "P1", comentarios iniciados por '#', qualquer
// espaco em branco entre tokens, e validar dimensoes.

namespace {

// Função auxiliar (anônima) para avançar o stream do arquivo ignorando
// espaços em branco e comentários iniciados por '#' de forma extremamente rápida.

void skip_whitespace_and_comments(std::ifstream& file) {
    int c;
    c = file.peek();
    while (c != EOF) {
        if(std::isspace(c)) {
            file.get();
        } else if (c == '#') {
            while (c != EOF && c != '\n') {
                c = file.get();
            }
        } else {
            break;
        }
        c = file.peek();
    }
}

}

Image read_pbm(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("read_pbm: Erro ao abrir arquivo de entrada: " + path);
    }

    // 1. Validar Magic Number
    skip_whitespace_and_comments(file);
    std::string magic;
    file >> magic;
    if (magic != "P1") {
        throw std::runtime_error("read_pbm: Formato invalido, esperado PBM ASCII (P1)");
    }

    // 2. Ler Dimensões
    skip_whitespace_and_comments(file);
    int width = 0;
    file >> width;

    skip_whitespace_and_comments(file);
    int height = 0;
    file >> height;

    if (width <= 0 || height <= 0) {
        throw std::runtime_error("read_pbm: Dimensoes invalidas ou corrompidas no arquivo");
    }

    Image img(width, height);

    // 3. Ler Pixels
    std::size_t total_pixels = static_cast<std::size_t>(width) * height;
    std::size_t loaded_pixels = 0;

    while (loaded_pixels < total_pixels) {
        skip_whitespace_and_comments(file);
        int pixel_char = file.get();
        
        if (pixel_char == EOF) break;

        if (pixel_char == '0' || pixel_char == '1') {
            img.data[loaded_pixels] = static_cast<std::uint8_t>(pixel_char - '0');
            loaded_pixels++;
        } else {
            throw std::runtime_error("read_pbm: Caractere invalido encontrado na matriz de pixels");
        }
    }

    // Validar se o arquivo não acabou antes de preencher a matriz prometida no cabeçalho
    if (loaded_pixels < total_pixels) {
        throw std::runtime_error("read_pbm: O arquivo terminou antes de todos os pixels serem lidos");
    }

    return img;
}

// [Membro 1] TODO: implementar escrita de PBM P1.
void write_pbm(const std::string& path, const Image& img) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("write_pbm: Erro ao criar ou gravar no arquivo de saida: " + path);
    }

    // Escrever cabeçalho
    file << "P1\n";
    file << img.width << " " << img.height << "\n";

    // Escrever os dados (pixels)
    int chars_in_current_line = 0;
    for (std::size_t i = 0; i < img.data.size(); i++) {
        file << static_cast<int>(img.data[i]);
        chars_in_current_line++;

        if (chars_in_current_line >= 70) {
            file << "\n";
            chars_in_current_line = 0;
        } else {
            file << " ";
        }
    }
    
    if (chars_in_current_line != 0) {
        file << "\n";
    }
}



} // namespace preocr   
