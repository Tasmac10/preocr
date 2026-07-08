// Helper de teste do modulo de ruido (Membro 2).
//
// Este arquivo NAO faz parte do binario principal (preocr). Ele tem seu proprio
// main e um leitor/gravador de PBM temporario, apenas para testar remove_noise()
// de forma isolada, enquanto a leitura oficial (Membro 1) nao fica pronta.
//
// Compilar:
//   g++ -std=c++17 -Iinclude tests/test_noise.cpp src/noise.cpp -o test_noise
// Usar:
//   ./test_noise <entrada.pbm> <saida.pbm> [referencia_limpa.pbm]

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "image.hpp"
#include "noise.hpp"

namespace {

// Avanca o fluxo pulando espacos em branco e linhas de comentario (iniciadas por '#').
void pular_espacos_e_comentarios(std::istream& fluxo) {
    while (true) {
        int proximo = fluxo.peek();
        if (proximo == '#') {
            std::string linha_ignorada;
            std::getline(fluxo, linha_ignorada);
        } else if (proximo == ' ' || proximo == '\n' || proximo == '\r' || proximo == '\t') {
            fluxo.get();
        } else {
            break;
        }
    }
}

// Le uma imagem PBM ASCII (P1). Leitor simplificado, so para os testes do modulo.
preocr::Image ler_pbm(const std::string& caminho) {
    std::ifstream arquivo(caminho);
    if (!arquivo) {
        throw std::runtime_error("nao foi possivel abrir: " + caminho);
    }

    std::string numero_magico;
    arquivo >> numero_magico;
    if (numero_magico != "P1") {
        throw std::runtime_error("formato invalido (esperado P1): " + caminho);
    }

    int largura = 0;
    int altura = 0;
    pular_espacos_e_comentarios(arquivo);
    arquivo >> largura;
    pular_espacos_e_comentarios(arquivo);
    arquivo >> altura;
    if (largura <= 0 || altura <= 0) {
        throw std::runtime_error("dimensoes invalidas em: " + caminho);
    }

    preocr::Image imagem(largura, altura);
    int total_pixels = largura * altura;
    int pixels_lidos = 0;
    char caractere = 0;
    while (pixels_lidos < total_pixels && arquivo.get(caractere)) {
        if (caractere == '0' || caractere == '1') {
            int coluna = pixels_lidos % largura;
            int linha = pixels_lidos / largura;
            imagem.at(coluna, linha) = (caractere == '1') ? 1 : 0;
            ++pixels_lidos;
        }
        // qualquer outro caractere (espaco, quebra de linha) e ignorado
    }
    if (pixels_lidos != total_pixels) {
        throw std::runtime_error("dados insuficientes no arquivo: " + caminho);
    }
    return imagem;
}

// Grava uma imagem PBM ASCII (P1): uma linha da imagem por linha do arquivo.
void gravar_pbm(const std::string& caminho, const preocr::Image& imagem) {
    std::ofstream arquivo(caminho);
    if (!arquivo) {
        throw std::runtime_error("nao foi possivel gravar: " + caminho);
    }
    arquivo << "P1\n" << imagem.width << " " << imagem.height << "\n";
    for (int linha = 0; linha < imagem.height; ++linha) {
        for (int coluna = 0; coluna < imagem.width; ++coluna) {
            arquivo << static_cast<int>(imagem.at(coluna, linha));
        }
        arquivo << "\n";
    }
}

// Conta quantos pixels diferem entre duas imagens de mesmas dimensoes.
int contar_pixels_diferentes(const preocr::Image& primeira, const preocr::Image& segunda) {
    if (primeira.width != segunda.width || primeira.height != segunda.height) {
        throw std::runtime_error("imagens com dimensoes diferentes na comparacao");
    }
    int diferentes = 0;
    for (int linha = 0; linha < primeira.height; ++linha) {
        for (int coluna = 0; coluna < primeira.width; ++coluna) {
            if (primeira.at(coluna, linha) != segunda.at(coluna, linha)) {
                ++diferentes;
            }
        }
    }
    return diferentes;
}

// Monta uma imagem branca com um bloco 3x3 preto em cada pixel onde as duas
// imagens diferem. O bloco 3x3 e so para o ponto ficar visivel quando a imagem
// e vista inteira; o pixel de diferenca real e o centro do bloco.
preocr::Image imagem_diferenca(const preocr::Image& filtrada, const preocr::Image& limpa) {
    preocr::Image diferenca(limpa.width, limpa.height);
    for (int linha = 0; linha < limpa.height; ++linha) {
        for (int coluna = 0; coluna < limpa.width; ++coluna) {
            if (filtrada.at(coluna, linha) != limpa.at(coluna, linha)) {
                for (int deslocamento_linha = -1; deslocamento_linha <= 1; ++deslocamento_linha) {
                    for (int deslocamento_coluna = -1; deslocamento_coluna <= 1; ++deslocamento_coluna) {
                        int vizinho_linha = linha + deslocamento_linha;
                        int vizinho_coluna = coluna + deslocamento_coluna;
                        if (diferenca.in_bounds(vizinho_coluna, vizinho_linha)) {
                            diferenca.at(vizinho_coluna, vizinho_linha) = 1;
                        }
                    }
                }
            }
        }
    }
    return diferenca;
}

// Insere um sufixo antes da extensao do caminho (ex.: out.pbm -> out_diff.pbm).
std::string nome_com_sufixo(const std::string& caminho, const std::string& sufixo) {
    std::string::size_type ponto = caminho.find_last_of('.');
    if (ponto == std::string::npos) {
        return caminho + sufixo;
    }
    return caminho.substr(0, ponto) + sufixo + caminho.substr(ponto);
}

} // namespace

// Uso: test_noise <entrada.pbm> <saida.pbm> [referencia_limpa.pbm]
int main(int argc, char** argv) {
    if (argc < 3 || argc > 4) {
        std::cerr << "Uso: " << argv[0]
                  << " <entrada.pbm> <saida.pbm> [referencia_limpa.pbm]\n";
        return 1;
    }

    const std::string caminho_entrada = argv[1];
    const std::string caminho_saida = argv[2];

    try {
        preocr::Image imagem_com_ruido = ler_pbm(caminho_entrada);
        preocr::Image imagem_filtrada = preocr::remove_noise(imagem_com_ruido);
        gravar_pbm(caminho_saida, imagem_filtrada);

        std::cout << "Imagem " << imagem_com_ruido.width << "x" << imagem_com_ruido.height
                  << " filtrada e gravada em " << caminho_saida << "\n";

        // Se uma imagem limpa de referencia for informada, mede o ruido antes e
        // depois do filtro (quantos pixels diferem da referencia).
        if (argc == 4) {
            const std::string caminho_referencia = argv[3];
            preocr::Image imagem_limpa = ler_pbm(caminho_referencia);
            int ruido_antes = contar_pixels_diferentes(imagem_com_ruido, imagem_limpa);
            int ruido_depois = contar_pixels_diferentes(imagem_filtrada, imagem_limpa);
            std::cout << "Pixels diferentes da referencia antes:  " << ruido_antes << "\n";
            std::cout << "Pixels diferentes da referencia depois: " << ruido_depois << "\n";
            if (ruido_antes > 0) {
                double reducao = 100.0 * (ruido_antes - ruido_depois) / ruido_antes;
                std::cout << "Reducao de ruido: " << reducao << "%\n";
            }

            // Grava a imagem de diferenca para inspecao visual dos pixels restantes.
            preocr::Image diferenca = imagem_diferenca(imagem_filtrada, imagem_limpa);
            std::string caminho_diferenca = nome_com_sufixo(caminho_saida, "_diff");
            gravar_pbm(caminho_diferenca, diferenca);
            std::cout << "Imagem de diferenca (pixels restantes destacados) em "
                      << caminho_diferenca << "\n";
        }
    } catch (const std::exception& erro) {
        std::cerr << "Erro: " << erro.what() << "\n";
        return 1;
    }

    return 0;
}
