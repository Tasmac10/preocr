#include "image.hpp"

#include <stdexcept>

namespace preocr {

// [Membro 1] TODO: implementar leitura de PBM P1.
// Deve tratar: magic number "P1", comentarios iniciados por '#', qualquer
// espaco em branco entre tokens, e validar dimensoes.
Image read_pbm(const std::string& path) {
    (void)path;
    throw std::runtime_error("read_pbm: nao implementado");
}

// [Membro 1] TODO: implementar escrita de PBM P1.
void write_pbm(const std::string& path, const Image& img) {
    (void)path;
    (void)img;
    throw std::runtime_error("write_pbm: nao implementado");
}

} // namespace preocr
