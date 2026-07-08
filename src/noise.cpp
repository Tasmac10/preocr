#include "noise.hpp"

namespace preocr {

// [Membro 2] TODO: implementar remocao de ruido sal-e-pimenta.
Image remove_noise(const Image& img) {
    Image saida(img.width, img.height);
    for (int altura = 0; altura < img.height; ++altura) {
        for (int largura = 0; largura < img.width; ++largura) {
            int vizinhos_pretos = 0;
            for (int deslocamento_altura = -1; deslocamento_altura <= 1; ++deslocamento_altura) {
                for (int deslocamento_largura = -1; deslocamento_largura <= 1; ++deslocamento_largura) {
                    if (deslocamento_altura == 0 && deslocamento_largura == 0) {
                        continue; // pula o proprio pixel central
                    }
                    int vizinho_altura = altura + deslocamento_altura;
                    int vizinho_largura = largura + deslocamento_largura;
                    if (img.in_bounds(vizinho_largura, vizinho_altura)) {
                        vizinhos_pretos += img.at(vizinho_largura, vizinho_altura);
                    }
                }
            }

            int pixel_atual = img.at(largura, altura);
            if (pixel_atual == 1 && vizinhos_pretos == 0 || pixel_atual == 1 && vizinhos_pretos == 1) {
                saida.at(largura, altura) = 0;            // remove pimenta
            } else if (pixel_atual == 0 && vizinhos_pretos == 8) {
                saida.at(largura, altura) = 1;            // preenche buraco (sal)
            } else {
                saida.at(largura, altura) = pixel_atual;  // mantem o original
            }
        }
    }
    return saida;
}
}