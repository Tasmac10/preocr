#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>

#include "image.hpp"
#include "noise.hpp"
#include "segmentation.hpp"
#include "words.hpp"

// Uso: preocr <entrada.pbm> [saida.pbm]
// Imprime no terminal o numero de colunas, linhas e palavras, e gera uma imagem
// PBM (saida.pbm por padrao) com cada palavra circunscrita por um retangulo.
int main(int argc, char** argv) {
    if (argc < 2 || argc > 3) {
        std::cerr << "Uso: " << argv[0] << " <entrada.pbm> [saida.pbm]\n";
        return EXIT_FAILURE;
    }

    const std::string entrada = argv[1];
    const std::string saida = (argc == 3) ? argv[2] : "saida.pbm";

    try {
        // [Membro 1] Leitura.
        preocr::Image img = preocr::read_pbm(entrada);

        // [Membro 2] Remocao de ruido.
        preocr::Image limpa = preocr::remove_noise(img);

        // [Membro 3] Segmentacao em colunas e linhas.
        std::vector<preocr::Column> colunas = preocr::segment(limpa);

        // [Membro 4] Deteccao de palavras + desenho dos retangulos.
        preocr::Image resultado = limpa;
        std::size_t total_linhas = 0;
        std::size_t total_palavras = 0;
        for (const preocr::Column& col : colunas) {
            total_linhas += col.lines.size();
            for (const preocr::Line& linha : col.lines) {
                std::vector<preocr::BBox> palavras =
                    preocr::detect_words(limpa, linha.box);
                total_palavras += palavras.size();
                for (const preocr::BBox& p : palavras) {
                    preocr::draw_rect(resultado, p);
                }
            }
        }

        // Saida no terminal.
        std::cout << "Colunas: " << colunas.size() << "\n";
        std::cout << "Linhas: " << total_linhas << "\n";
        std::cout << "Palavras: " << total_palavras << "\n";

        // [Membro 1] Escrita da imagem com os retangulos.
        preocr::write_pbm(saida, resultado);
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
