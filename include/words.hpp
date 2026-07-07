#ifndef PREOCR_WORDS_HPP
#define PREOCR_WORDS_HPP

#include <vector>

#include "image.hpp"

namespace preocr {

// [Membro 4] Detecta as palavras dentro de uma linha e retorna seus retangulos
// delimitadores. Estrategia: espacamento horizontal / componentes conexos.
std::vector<BBox> detect_words(const Image& img, const BBox& line);

// [Membro 4] Desenha o contorno de um retangulo na imagem (pixels = 1).
void draw_rect(Image& img, const BBox& box);

} // namespace preocr

#endif // PREOCR_WORDS_HPP
