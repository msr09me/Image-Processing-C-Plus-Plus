#ifndef IMAGE_TRANSFORMS_H
#define IMAGE_TRANSFORMS_H

#include <stdint.h> // For uint8_t
#include "ImageIO.h"

void applyNegative(uint8_t *buffer, const ImageMetadata &meta);
void applyLogTransform(uint8_t *buffer, const ImageMetadata &meta);
void applyGammaTransform(uint8_t *buffer, const ImageMetadata &meta);

#endif // IMAGE_TRANSFORMS_H