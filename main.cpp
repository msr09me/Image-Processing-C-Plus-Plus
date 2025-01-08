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

    ImageMetadata meta;

    std::cout << "Attempting to read input image from: " << inputImage << std::endl;

    ImageReadResult result = readImage(inputImage, meta);

    if (!result.buffer) {
        std::cerr << "Failed to read the input image." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Image successfully read. Dimensions: " << meta.width << "x" << meta.height
              << ", Bit Depth: " << meta.bitDepth << std::endl;

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
                applyNegative(result.buffer->data(), meta);
                break;
            }
            case 2: {
                std::cout << "Log Transformation" << std::endl;
                applyLogTransform(result.buffer->data(), meta);
                break;
            }
            case 3: {
                std::cout << "Gamma Transformation" << std::endl;
                applyGammaTransform(result.buffer->data(), meta);
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
                int histogram[65536] = {0};
                calculateHistogram(result.buffer->data(), meta.width, meta.height, meta.bitDepth, histogram, -1);
                displayHistogram(histogram, meta.bitDepth, false,'N');
                std::string outputFileName = "histogram_data.csv";
                saveHistogramToFile(histogram, meta.bitDepth, outputFileName);
                displayHistogramAsBarChart(histogram, meta.bitDepth);
            } else if (histogramType == 2) {
                if (meta.bitDepth != 24) {
                    std::cerr << "RGB histogram is only supported for 24-bit images." << std::endl;
                    break;
                }
                int redHistogram[256] = {0};
                int greenHistogram[256] = {0};
                int blueHistogram[256] = {0};

                calculateHistogram(result.buffer->data(), meta.width, meta.height, 8, redHistogram, 0);   // Red channel
                calculateHistogram(result.buffer->data(), meta.width, meta.height, 8, greenHistogram, 1); // Green channel
                calculateHistogram(result.buffer->data(), meta.width, meta.height, 8, blueHistogram, 2);  // Blue channel

                displayHistogram(redHistogram, 8, true, 'R');
                displayHistogram(greenHistogram, 8, true, 'G');
                displayHistogram(blueHistogram, 8, true, 'B');

                saveHistogramToFile(redHistogram, 8, "redHistogram_data.csv");
                saveHistogramToFile(greenHistogram, 8, "greenHistogram_data.csv");
                saveHistogramToFile(blueHistogram, 8, "blueHistogram_data.csv");

                displayHistogramAsBarChart(redHistogram, 8);
                displayHistogramAsBarChart(greenHistogram, 8);
                displayHistogramAsBarChart(blueHistogram, 8);
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
    if (!writeImage(outputImage, meta, result.header, result.colorTable, *result.buffer)) {
        std::cerr << "Failed to write the output image." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Image processing completed successfully." << std::endl;
    return EXIT_SUCCESS;

}
