#include "words.hpp"

namespace preocr {

// [Membro 4] TODO: implementar deteccao de palavras dentro de uma linha.
std::vector<BBox> detect_words(const Image& img, const BBox& line) {
    (void)img;
    (void)line;
    return {}; // stub
}

// [Membro 4] TODO: implementar desenho do contorno do retangulo.
void draw_rect(Image& img, const BBox& box) {
    (void)img;
    (void)box;
}

} // namespace preocr
