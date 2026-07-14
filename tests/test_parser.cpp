#include <iostream>
#include <cassert>
#include "../include/image.hpp"

int main() {
    try {
        std::cout << "[TESTE] Iniciando leitura do PBM...\n";
        preocr::Image img = preocr::read_pbm("images/input/teste_manual.pbm");

        std::cout << "[TESTE] Dimensoes lidas: " << img.width << "x" << img.height << "\n";
        assert(img.width == 3);
        assert(img.height == 5);
        assert(img.at(0, 0) == 0); assert(img.at(1, 0) == 1); assert(img.at(2, 0) == 0);
        assert(img.at(1, 4) == 1);

        std::cout << "[TESTE] Leitura e dados internos: Ok\n";

        std::cout << "[TESTE] Escrevendo copia_teste.pbm...\n";
        preocr::write_pbm("copia_teste.pbm", img);

        preocr::Image img_copia = preocr::read_pbm("copia_teste.pbm");
        assert(img_copia.width == img.width);
        assert(img_copia.height == img.height);
        assert(img_copia.data == img.data);

        std::cout << "\n>>> SUCESSO! O parser passou em todas as validacoes. <<<\n";
    } catch (const std::exception& e) {
        std::cerr << "\n>>> FALHA NO TESTE: " << e.what() << " <<<\n";
        return 1;
    }
    return 0;
}