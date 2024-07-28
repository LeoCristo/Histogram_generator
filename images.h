#ifndef IMAGE_H
#define IMAGE_H

#include <stdint.h>

// Definições para a primeira imagem
#define WIDTH0  4
#define HEIGHT0 3
extern const uint8_t image0[HEIGHT0][WIDTH0];

// Definições para a segunda imagem
#define WIDTH1  160
#define HEIGHT1 120

extern const uint8_t image1[HEIGHT1][WIDTH1];
extern const uint8_t * const p_start_image1;

#endif // IMAGE_H
