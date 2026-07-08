#include "noise.hpp"

#include <cstddef>
#include <vector>

namespace preocr {

namespace {

// Tamanho maximo (em pixels) de um componente preto tratado como ruido.
// Blobs pretos isolados com ate este tamanho sao removidos; letras, que sao
// muito maiores (altura minima de 12 pixels), sao preservadas.
// O enunciado garante ruido de 1 pixel; usamos 2 para tambem pegar os pares de
// pixels adjacentes observados nas imagens de teste.
constexpr int tamanho_maximo_ruido = 2;

} // namespace

// [Membro 2] Remove ruido sal-e-pimenta de uma imagem binaria.
//
// Passo 1 (pimenta): via componentes conexos, apaga todo blob preto isolado com
//   tamanho <= tamanho_maximo_ruido. Como olha o objeto inteiro (nao apenas a
//   vizinhanca), remove blobs de 1 e 2 pixels sem comer pontas de letras: uma
//   ponta pertence ao componente da letra inteira, que e grande.
// Passo 2 (sal): preenche buracos brancos de 1 pixel cercados por 8 vizinhos
//   pretos.
//
// Os dois passos leem sempre da imagem ORIGINAL (img) e escrevem em saida, para
// que uma correcao nao contamine a decisao sobre os pixels seguintes.
Image remove_noise(const Image& img) {
    Image saida = img;

    // --- Passo 1: remove componentes pretos pequenos (pimenta) ---
    std::vector<bool> visitados(static_cast<std::size_t>(img.width) * img.height, false);
    std::vector<int> pilha;       // fronteira do flood fill (indices = linha*largura + coluna)
    std::vector<int> componente;  // pixels do componente atual

    for (int linha = 0; linha < img.height; ++linha) {
        for (int coluna = 0; coluna < img.width; ++coluna) {
            int indice_inicial = linha * img.width + coluna;
            if (img.at(coluna, linha) == 0 || visitados[indice_inicial]) {
                continue; // so inicia em pixel preto ainda nao visitado
            }

            // Flood fill 8-conexo com pilha explicita, marcando ao empilhar.
            componente.clear();
            pilha.clear();
            visitados[indice_inicial] = true;
            pilha.push_back(indice_inicial);

            while (!pilha.empty()) {
                int indice = pilha.back();
                pilha.pop_back();
                componente.push_back(indice);

                int x = indice % img.width;
                int y = indice / img.width;
                for (int deslocamento_y = -1; deslocamento_y <= 1; ++deslocamento_y) {
                    for (int deslocamento_x = -1; deslocamento_x <= 1; ++deslocamento_x) {
                        if (deslocamento_x == 0 && deslocamento_y == 0) {
                            continue;
                        }
                        int vizinho_x = x + deslocamento_x;
                        int vizinho_y = y + deslocamento_y;
                        if (!img.in_bounds(vizinho_x, vizinho_y)) {
                            continue;
                        }
                        int indice_vizinho = vizinho_y * img.width + vizinho_x;
                        if (img.at(vizinho_x, vizinho_y) == 1 && !visitados[indice_vizinho]) {
                            visitados[indice_vizinho] = true;
                            pilha.push_back(indice_vizinho);
                        }
                    }
                }
            }

            // Componente pequeno = ruido: apaga todos os seus pixels.
            if (static_cast<int>(componente.size()) <= tamanho_maximo_ruido) {
                for (int indice : componente) {
                    saida.data[indice] = 0;
                }
            }
        }
    }

    // --- Passo 2: preenche buracos de 1 pixel (sal) dentro de regioes pretas ---
    for (int linha = 0; linha < img.height; ++linha) {
        for (int coluna = 0; coluna < img.width; ++coluna) {
            if (img.at(coluna, linha) != 0) {
                continue;
            }
            int vizinhos_pretos = 0;
            for (int deslocamento_y = -1; deslocamento_y <= 1; ++deslocamento_y) {
                for (int deslocamento_x = -1; deslocamento_x <= 1; ++deslocamento_x) {
                    if (deslocamento_x == 0 && deslocamento_y == 0) {
                        continue;
                    }
                    if (img.in_bounds(coluna + deslocamento_x, linha + deslocamento_y)) {
                        vizinhos_pretos += img.at(coluna + deslocamento_x, linha + deslocamento_y);
                    }
                }
            }
            if (vizinhos_pretos == 8) {
                saida.at(coluna, linha) = 1;
            }
        }
    }

    return saida;
}

} // namespace preocr
