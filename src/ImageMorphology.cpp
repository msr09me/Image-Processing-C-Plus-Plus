#include "ImageMorphology.h"
#include <algorithm>
#include <cassert>

// Erosion
std::vector<uint8_t> applyErosion(const ImageReadResult& inputImage, int kernelColumns, int kernelRows) {
    const uint8_t* buffer = inputImage.buffer->data();
    const ImageMetadata& meta = inputImage.meta;

    int rows = meta.height;
    int cols = meta.width;
    int halfKernelColumns = kernelColumns / 2;
    int halfKernelRows = kernelRows / 2;

    std::vector<uint8_t> outputBuffer(rows * cols, 255);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            uint8_t minValue = 255;

            for (int ki = -halfKernelRows; ki <= halfKernelRows; ++ki) {
                for (int kj = -halfKernelColumns; kj <= halfKernelColumns; ++kj) {
                    int x = i + ki;
                    int y = j + kj;

                    if (x >= 0 && x < rows && y >= 0 && y < cols) {
                        minValue = std::min(minValue, buffer[x * cols + y]);
                    }
                }
            }

            outputBuffer[i * cols + j] = minValue;
        }
    }

    return outputBuffer;
}

// Dilation
std::vector<uint8_t> applyDilation(const ImageReadResult& inputImage, int kernelColumns, int kernelRows) {
    const uint8_t* buffer = inputImage.buffer->data();
    const ImageMetadata& meta = inputImage.meta;

    int rows = meta.height;
    int cols = meta.width;
    int halfKernelColumns = kernelColumns / 2;
    int halfKernelRows = kernelRows / 2;

    std::vector<uint8_t> outputBuffer(rows * cols, 255);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            uint8_t maxValue = 0;

            for (int ki = -halfKernelRows; ki <= halfKernelRows; ++ki) {
                for (int kj = -halfKernelColumns; kj <= halfKernelColumns; ++kj) {
                    int x = i + ki;
                    int y = j + kj;

                    if (x >= 0 && x < rows && y >= 0 && y < cols) {
                        maxValue = std::max(maxValue, buffer[x * cols + y]);
                    }
                }
            }

            outputBuffer[i * cols + j] = maxValue;
        }
    }

    return outputBuffer;
}

// Opening: Erosion followed by Dilation
std::vector<uint8_t> applyOpening(const ImageReadResult& inputImage, int kernelColumns, int kernelRows) {
    ImageReadResult tempImage = inputImage;
    tempImage.buffer = applyErosion(inputImage, kernelColumns, kernelRows);

    return applyDilation(tempImage, kernelColumns, kernelRows);
}

// Closing: Dilation followed by Erosion
std::vector<uint8_t> applyClosing(const ImageReadResult& inputImage, int kernelColumns, int kernelRows) {
    ImageReadResult tempImage = inputImage;
    tempImage.buffer = applyDilation(inputImage, kernelColumns, kernelRows);

    return applyErosion(tempImage, kernelColumns, kernelRows);
}

// Boundary extraction: Erosion followed by set difference
std::vector<uint8_t> applyBoundaryExtraction(const ImageReadResult& inputImage, int kernelColumns, int kernelRows){

    const uint8_t *buffer = inputImage.buffer->data();
    const ImageMetadata &meta = inputImage.meta;

    int rows = meta.height;
    int cols = meta.width;

    // output buffer
    std::vector<uint8_t> boundaryImageBuffer(rows * cols, 0);

    // Perform the erosion
    std::vector<uint8_t> erodedImage = applyErosion(inputImage, kernelColumns, kernelRows);

    // Perform the set difference
    for (int i = 0; i < rows*cols; ++i)
    {
        // boundaryImageBuffer[i] = std::clamp(static_cast<int>(butffer[i]) - static_cast<int>(erodedImage[i]), 0, 255);
        // cannot use the clamp though I use c++17

        boundaryImageBuffer[i] = std::max(0, std::min(255, static_cast<int>(buffer[i]) - static_cast<int>(erodedImage[i])));

    }

    return boundaryImageBuffer;
    
}

// Hole Filling Implementation

// incomplete testing
std::vector<uint8_t> applyHoleFilling(const ImageReadResult& inputImage, const std::pair<int, int>& seedPoint, int kernelColumns, int kernelRows) {
    const uint8_t* buffer = inputImage.buffer->data();
    const ImageMetadata& meta = inputImage.meta;

    int rows = meta.height;
    int cols = meta.width;

    // Step 1: Initialize seed image, X_0
    std::vector<uint8_t> seedImage(rows * cols, 0);
    seedImage[seedPoint.first * cols + seedPoint.second] = 255;

    // Step 2: Compute complement of the input image
    std::vector<uint8_t> complementImage(rows * cols, 0);
    for (int i = 0; i < rows * cols; ++i) {
        complementImage[i] = (buffer[i] == 0) ? 255 : 0;
    }

    // Step 3: Iterative dilation and intersection
    std::vector<uint8_t> currentImage = seedImage;      // X_k
    std::vector<uint8_t> nextImage(rows * cols, 0);     // X_(k+1)
    bool hasChanged;

    int dilationCount = 0;

    do {
        hasChanged = false;

        // Perform dilation
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (currentImage[i * cols + j] == 255) {
                    for (int ki = -kernelRows / 2; ki <= kernelRows / 2; ++ki) {
                        for (int kj = -kernelColumns / 2; kj <= kernelColumns / 2; ++kj) {
                            int x = i + ki;
                            int y = j + kj;
                            if (x >= 0 && x < rows && y >= 0 && y < cols) {
                                if (complementImage[x * cols + y] == 255 && nextImage[x * cols + y] == 0) {
                                    nextImage[x * cols + y] = 255;
                                    hasChanged = true;
                                }
                            }
                        }
                    }
                }
            }
        }

        std::cout << "Dilation Step " << dilationCount + 1 << ": Filled Pixels = " 
                  << std::count(nextImage.begin(), nextImage.end(), 255) << std::endl;

        currentImage = nextImage;
        dilationCount++;

    } while (hasChanged);

    // Step 4: Add filled region to the original image
    std::vector<uint8_t> filledImage(rows * cols, 0);
    for (int i = 0; i < rows * cols; ++i) {
        // Copy the original image
        if (buffer[i] == 255) {
            filledImage[i] = 255; // Preserve original foreground
        } else if (currentImage[i] == 255) {
            filledImage[i] = 255; // Add the filled hole
        } else {
            filledImage[i] = 0; // Preserve original background
        }
    }

    // Debugging: Verify final image values
    int foregroundPixels = std::count(filledImage.begin(), filledImage.end(), 255);
    int backgroundPixels = std::count(filledImage.begin(), filledImage.end(), 0);
    std::cout << "Filled Image: Background Pixels = " << backgroundPixels 
              << ", Foreground Pixels = " << foregroundPixels << std::endl;

              // Debugging: Verify final image values
    int foregroundPixelsb = std::count(filledImage.begin(), filledImage.end(), 255);
    int backgroundPixelsb = std::count(filledImage.begin(), filledImage.end(), 0);
    std::cout << "buffer Image: Background Pixels = " << backgroundPixelsb 
              << ", Foreground Pixels = " << foregroundPixelsb << std::endl;

    // Debugging: Verify final image values
    int foregroundPixelsc = std::count(currentImage.begin(), currentImage.end(), 255);
    int backgroundPixelsc = std::count(currentImage.begin(), currentImage.end(), 0);
    std::cout << "current image: Background Pixels = " << backgroundPixelsc 
              << ", Foreground Pixels = " << foregroundPixelsc << std::endl;

    return filledImage;

    //return currentImage;
}


