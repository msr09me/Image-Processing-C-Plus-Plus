#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <optional>
#include <vector>
#include "ImageIO.h"
#include "IntensityTransformations.h"
#include "ImageHistogram.h"

int main() {
    const std::string inputImage = "../TestImages/lena512.bmp";
    const std::string outputImage = "../TestImages/lena512_output.bmp";

    std::cout << "Attempting to read input image from: " << inputImage << std::endl;

    ImageReadResult result = readImage(inputImage);

    if (!result.buffer) {
        std::cerr << "Failed to read the input image." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Image successfully read. Dimensions: " << result.meta.width << "x" << result.meta.height
              << ", Bit Depth: " << result.meta.bitDepth << std::endl;

    std::cout << "From the following options, what do you want to do?\n"
              << "1. Intensity Transformation\n"
              << "2. Histogram\n"
              << "Type the number: ";

    int choice1;
    std::cin >> choice1;

    switch (choice1)
    {
    case 1:
        std::cout << "What type of transformation do you want to perform?\n"
                << "1. Negative Transformation\n"
                << "2. Log Transformation\n"
                << "3. Gamma Transformation\n"
                << "Type the number: ";

        int choice2;
        std::cin >> choice2;
        if (std::cin.fail()) {
                std::cerr << "Invalid input. Exiting." << std::endl;
                return EXIT_FAILURE;
            }

        switch (choice2) {
            case 1: {
                std::cout << "Negative Transformation" << std::endl;
                applyNegative(result.buffer->data(), result.meta);
                break;
            }
            case 2: {
                std::cout << "Log Transformation" << std::endl;
                applyLogTransform(result.buffer->data(), result.meta);
                break;
            }
            case 3: {
                std::cout << "Gamma Transformation" << std::endl;
                applyGammaTransform(result.buffer->data(), result.meta);
                break;
            }
            default:
                std::cerr << "Invalid choice." << std::endl;
                return EXIT_FAILURE;
        }
        break;

    case 2:
        std::cout << "What histogram option do you want?\n"
                << "1. Histogram Calculation\n"
                << "2. Histogram Equatiization\n"
                << "3. Histogram Specification\n"
                << "Type the number:";

        std::cin >> choice2;

        switch (choice2)
        {
        case 1:

            std::cout << "Choose the type of histogram to calculate:\n"
              << "1. Grayscale Image Histogram\n"
              << "2. RGB Image Histogram\n"
              << "Type the number: ";

            int histogramType;
            std::cin >> histogramType;

            std::cout << "Calculating histogram..." <<std::endl;

            if (histogramType == 1) {
                grayscaleHistogram(result);
            } else if (histogramType == 2) {
                rgbHistogram(result);
            } else {
                std::cerr << "Invalid option." << std::endl;
            }
            break;
        
        default:
            break;
        }
        break;    
    
    default:
        break;
    }

    

    // Write the image to a new file
    if (!writeImage(outputImage, result)) {
        std::cerr << "Failed to write the output image." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Image processing completed successfully." << std::endl;
    return EXIT_SUCCESS;

}
