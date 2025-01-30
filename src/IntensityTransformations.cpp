#include "IntensityTransformations.h"
#include <cmath> // For log, pow

void applyNegative(uint8_t *buffer, const ImageMetadata &meta) {

    std::cout << "Aplying Negative Transformation...\n";

    int maxVal = (1 << meta.bitDepth) - 1;
    for (int i = 0; i < meta.width * meta.height; i++) {
        buffer[i] = maxVal - buffer[i];
    }

    std::cout << "Completed Negative Transformation...\n";
}

void applyLogTransform(uint8_t *buffer, const ImageMetadata &meta) {
    int maxVal = (1 << meta.bitDepth) - 1;

    double c = 0.0;
    std::cout << "Type the C value (type -1 if you want to use the default value): ";
    std::cin >> c;
    if (c == -1)
    {
        c = 255.0 / log(1 + 255.0); //default value
    }

    std::cout << "\n Applying Log Transformation...\n";

    for (int i = 0; i < meta.width * meta.height; i++) {
        double transformedValue = c * log(1 + buffer[i]);
        if (transformedValue > maxVal) {
            transformedValue = maxVal; // Clamp the value
        }
        buffer[i] = static_cast<uint8_t>(transformedValue);
    }

    std::cout << "Completed Log Transformation...\n";
}

void applyGammaTransform(uint8_t *buffer, const ImageMetadata &meta) {
    int maxVal = (1 << meta.bitDepth) - 1;

    double c = 0.0;
    std::cout << "Type the C value (type -1 if you want to use the default value): ";
    std::cin >> c;
    if (c == -1)
    {
        c = 255.0 / log(1 + 255.0);
    }

    double gamma = 0.1; //default value
    std::cout << "\nType the Gamma value (type -1 if you want to use the default value): ";
    std::cin >> c;
    if (gamma == -1)
    {
        gamma = 0.1;
    }

    std::cout << "\n Applying Gamma Transformation...\n";

    for (int i = 0; i < meta.width * meta.height; i++) {
        double transformedValue = c * pow(buffer[i], gamma);
        if (transformedValue > maxVal) {
            transformedValue = maxVal; // Clamp the value
        }
        buffer[i] = static_cast<uint8_t>(transformedValue);
    }

    std::cout << "Completed Gamma Transformation...\n";
}
