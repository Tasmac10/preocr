#ifndef PREOCR_SEGMENTATION_HPP
#define PREOCR_SEGMENTATION_HPP

#include <vector>

#include "image.hpp"

namespace preocr {

// Uma linha de texto dentro de uma coluna.
struct Line {
    BBox box;
};

// Uma coluna de texto, contendo suas linhas.
struct Column {
    BBox box;
    std::vector<Line> lines;
};

// [Membro 3] Segmenta a imagem em colunas e, dentro de cada coluna, em linhas.
// Colunas: projecao/gaps verticais. Linhas: projecao horizontal por coluna.
std::vector<Column> segment(const Image& img);

} // namespace preocr

#endif // PREOCR_SEGMENTATION_HPP
