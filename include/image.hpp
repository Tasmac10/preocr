#ifndef PREOCR_IMAGE_HPP
#define PREOCR_IMAGE_HPP

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace preocr {

// Retangulo delimitador (bounding box), coordenadas inclusivas em pixels.
struct BBox {
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    int width() const { return x1 - x0 + 1; }
    int height() const { return y1 - y0 + 1; }
};

// Imagem binaria em PBM P1. data em row-major; 1 = pixel preto (texto), 0 = branco.
struct Image {
    int width = 0;
    int height = 0;
    std::vector<std::uint8_t> data;

    Image() = default;
    Image(int w, int h) : width(w), height(h), data(static_cast<std::size_t>(w) * h, 0) {}

    std::uint8_t at(int x, int y) const {
        return data[static_cast<std::size_t>(y) * width + x];
    }
    std::uint8_t& at(int x, int y) {
        return data[static_cast<std::size_t>(y) * width + x];
    }
    bool in_bounds(int x, int y) const {
        return x >= 0 && x < width && y >= 0 && y < height;
    }
};

// [Membro 1] Le uma imagem PBM ASCII (P1). Lanca std::runtime_error em caso de
// erro (arquivo inexistente, magic number invalido, dimensoes incoerentes).
Image read_pbm(const std::string& path);

// [Membro 1] Escreve uma imagem PBM ASCII (P1).
void write_pbm(const std::string& path, const Image& img);

} // namespace preocr

#endif // PREOCR_IMAGE_HPP
