#include <iostream>
#include <cstdlib>
#include <string>
#include <cmath>
#include <optional>
#include <vector>
#include "ImageIO.h"
#include "IntensityTransformations.h"
#include "ImageHistogram.h"
#include "ImageFilter.h"

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
              << "3. Spatial Filtering\n"
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

        case 2:

            std::cout << "Histogram Equalization\n";

            result.buffer =  histogramEqualization(result);

            break;
        
        default:
            break;
        }
        break;

    case 3:
        std::cout << "What type of spatial filtering do you want?\n"
                    << "1. Lowpass filter\n"
                    << "2. Highpass filter\n"
                    << "3. Image sharpening\n"
                    << "Type the number: ";

        std::cin >> choice2;

        switch (choice2)
        {
        case 1:
            std::cout << "What type of lowpass filtering do you want?\n"
                    << "1. Box filter\n"
                    << "2. Gaussian filter\n"
                    << "3. Median filter\n"
                    << "Type the number: ";

            int lowPassFilterType;
            std::cin >> lowPassFilterType;

            if (lowPassFilterType == 1)
            {
                std::cout << "Enter the kernel size for the Box Filter: ";
                int kernelSize;
                std::cin >> kernelSize;

                if (kernelSize <= 0 || kernelSize % 2 == 0) {
                    std::cerr << "Kernel size must be a positive odd number." << std::endl;
                    break;
                }

                std::cout << "Applying Box Filter with kernel size " << kernelSize << "..." << std::endl;

                try {
                    // Call the function and get the filtered buffer
                    std::vector<uint8_t> filteredBuffer = applyBoxFilter(result, kernelSize);
                    
                    // Update the result buffer with the new filtered buffer
                    result.buffer = filteredBuffer;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            }else if (lowPassFilterType == 2)
            {
                std::cout << "Enter the kernel size for the Gaussian Filter: ";
                int kernelSize;
                std::cin >> kernelSize;

                if (kernelSize <= 0 || kernelSize % 2 == 0) {
                    std::cerr << "Kernel size must be a positive odd number." << std::endl;
                    break;
                }

                std::cout << "Enter the value of sigma: ";
                double sigma;
                std::cin >> sigma;

                std::cout << "Applying Gaussian Filter with kernel size " << kernelSize << " and sigma  " << sigma << "..." << std::endl;

                // Using the applyGaussianFilter function
                std::vector<uint8_t> filteredBuffer = applyGaussianFilter(result, kernelSize, sigma);

                // Store the filtered buffer into the result
                result.buffer = filteredBuffer;

            }else if (lowPassFilterType == 3)
            {
                std::cout << "Enter the kernel size for the Median Filter: ";
                int kernelSize;
                std::cin >> kernelSize;

                if (kernelSize <= 0 || kernelSize % 2 == 0) {
                    std::cerr << "Kernel size must be a positive odd number." << std::endl;
                    break;
                }

                std::cout << "Applying Median Filter with kernel size " << kernelSize << std::endl;

                // Using the applyGaussianFilter function
                std::vector<uint8_t> filteredBuffer = applyMedianFilter(result, kernelSize);

                // Store the filtered buffer into the result
                result.buffer = filteredBuffer;
            }
            break;

        case 2:
            std::cout << "Choose the kernel - \n"
                    << "1. Basic Laplacian\n"
                    << "2. Full Laplacian\n"
                    << "3. Basic Inverted Laplacian\n"
                    << "4. Full Inverted Laplacian\n"
                    << "Type the number: ";

            int kernelChoice;
            std::cin >> kernelChoice;

            std::cout << "Applying Highpass Filter ..." << std::endl;

                try {
                    // Call the function and get the filtered buffer
                    std::vector<uint8_t> filteredBuffer = applyHighPassFilter(result, kernelChoice);
                    
                    // Update the result buffer with the new filtered buffer
                    result.buffer = filteredBuffer;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
        
        case 3:
            std::cout << "Choose the kernel - \n"
                    << "1. Basic Laplacian\n"
                    << "2. Full Laplacian\n"
                    << "3. Basic Inverted Laplacian\n"
                    << "4. Full Inverted Laplacian\n"
                    << "Type the number: ";

            std::cin >> kernelChoice;

            std::cout << "Applying Image Sharpening ..." << std::endl;

                try {
                    // Call the function and get the filtered buffer
                    std::vector<uint8_t> filteredBuffer = applyImageSharpening(result, kernelChoice);
                    
                    // Update the result buffer with the new filtered buffer
                    result.buffer = filteredBuffer;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
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
