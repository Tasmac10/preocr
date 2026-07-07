#ifndef PREOCR_NOISE_HPP
#define PREOCR_NOISE_HPP

#include "image.hpp"

namespace preocr {

// [Membro 2] Remove ruido sal-e-pimenta (de 1 pixel) da imagem binaria.
// Implementar filtro (mediana/maioria em vizinhanca) pelo grupo, sem bibliotecas.
Image remove_noise(const Image& img);

} // namespace preocr

#endif // PREOCR_NOISE_HPP
