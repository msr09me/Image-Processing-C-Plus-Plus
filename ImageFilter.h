#ifndef IMAGE_FILTER_H
#define IMAGE_FILTER_H

#include "ImageIO.h"
#include <vector>
#include <stdexcept>
#include <cmath>
#include <algorithm>

// Apply Box Filter Function
std::vector<uint8_t> applyBoxFilter(const ImageReadResult& inputImage, int kernelSize);

// Apply Gaussian Filter Function
std::vector<uint8_t> applyGaussianFilter(const ImageReadResult& inputImage, int kernelSize, double sigma);

// Apply Median Filter
std::vector<uint8_t> applyMedianFilter(const ImageReadResult& inputImage, int kernelSize);

// High-pass filter with dynamic kernel selection
std::vector<uint8_t> applyHighPassFilter(const ImageReadResult& inputImage, int kernelChoice);

// Image sharpening
std::vector<uint8_t> applyImageSharpening(const ImageReadResult& inputImage, int kernelChoice);

#endif // IMAGE_FILTERS_H
