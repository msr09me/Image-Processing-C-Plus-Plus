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
#include "ImageConverter.h"
#include "ImageMorphology.h"
#include "ImageUtils.h"
#include "ImageEdgeDetection.h"

int main() {
    //const std::string inputImage = "../TestImages/Binary_Geometric_Shapes.bmp";
    //const std::string inputImage = "../TestImages/boundaryExtraction.bmp"; 
    const std::string inputImage = "../TestImages/HoleFilling_8bit_converted.bmp";
    const std::string outputImage = "../TestImages/HoleFilling_8bit_output.bmp";

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
              << "4. Imager Conversion\n"
              << "5. Image Morphology\n"
              << "6. Edge Detection\n"
              << "Type the number: ";

    int choice1;
    std::cin >> choice1;

    switch (choice1)
    {
    case 1: {
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
    }
    case 2: {
        std::cout << "What histogram option do you want?\n"
                << "1. Histogram Calculation\n"
                << "2. Histogram Equatiization\n"
                << "3. Histogram Specification\n"
                << "Type the number:";

        int choice2;
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
    }
    case 3: {
        std::cout << "What type of spatial filtering do you want?\n"
                    << "1. Lowpass filter\n"
                    << "2. Highpass filter\n"
                    << "3. Image sharpening\n"
                    << "4. Unsharp masking and Highboost filter\n"
                    << "Type the number: ";

        int choice2;
        std::cin >> choice2;

        std::vector<uint8_t> filteredBuffer;

        switch (choice2)
        {
        case 1: {
            std::cout << "Lowpass filter selected" <<std::endl;

            try {
                    filteredBuffer = lowPassFilter(result);
                    result.buffer = filteredBuffer; // the extra filteredBuffer take extra memory but we use this for debuggin issue
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            break; 
        }

        case 2: {
            std::cout << "Choose the kernel - \n"
                    << "1. Basic Laplacian\n"
                    << "2. Full Laplacian\n"
                    << "3. Basic Inverted Laplacian\n"
                    << "4. Full Inverted Laplacian\n"
                    << "5. Sobel Operator\n"
                    << "Type the number: ";

            int kernelChoice;
            std::cin >> kernelChoice;

            std::cout << "Applying Highpass Filter ..." << std::endl;

                try {
                    // Call the function and get the filtered buffer
                    filteredBuffer = applyHighPassFilter(result, kernelChoice);
                    
                    // Update the result buffer with the new filtered buffer
                    result.buffer = filteredBuffer;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            break; 
        }
        
        case 3: {
            std::cout << "Choose the kernel - \n"
                    << "1. Basic Laplacian\n"
                    << "2. Full Laplacian\n"
                    << "3. Basic Inverted Laplacian\n"
                    << "4. Full Inverted Laplacian\n"
                    << "5. Sobel Operator\n"
                    << "Type the number: ";

            int kernelChoice;
            std::cin >> kernelChoice;

            std::cout << "Applying Image Sharpening ..." << std::endl;

                try {
                    // Call the function and get the filtered buffer
                    filteredBuffer = applyImageSharpening(result, kernelChoice);
                    
                    // Update the result buffer with the new filtered buffer
                    result.buffer = filteredBuffer;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            break; 
        }
        case 4: {
            std::cout << "Unsharp masking and highboost filtering is selected" <<std::endl;
            double k = 0.0;

            std::cout << "Enter the k value: ";
            std::cin >> k;
            std::cout << std::endl;

            try {
                    filteredBuffer = applyUMHBF(result, k);
                    result.buffer = filteredBuffer; // the extra filteredBuffer take extra memory but we use this for debuggin issue
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            break; 
        }
        default:
            break;
        }
        
        break;
    }
    case 4: {
        std::cout << "What type of conversion you want to perform?\n"
                    << "1. Grayscale to Binary\n"
                    << "Type the number: ";

        int conversionChoice;
        std::cin >> conversionChoice;
        std::cout << std::endl;

        std::vector<uint8_t> filteredBuffer;

        if (conversionChoice == 1)
        {
            std::cout << "Enter the threshold value:";
            int thresholdValue;
            std::cin >> thresholdValue;
            try {
                    filteredBuffer = applyGrayscaleToBinary(result, thresholdValue);
                    result.buffer = filteredBuffer; // the extra filteredBuffer take extra memory but we use this for debugging issue
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
        }
        break;
    }
    case 5: {
        std::cout << "What type of morphological operation do you want to perform?\n"
              << "1. Erosion\n"
              << "2. Dilation\n"
              << "3. Opening\n"
              << "4. Closing\n"
              << "5. Boundary Extraction\n"
              << "6. Hole Filling\n"
              << "Type the number: ";

        int morphChoice;
        std::cin >> morphChoice;

        std::cout << "Enter the size of the structuring element (kernel size) " <<std::endl;
        std::cout << "Enter the row size: ";
        int kernelRowSize;
        std::cin >> kernelRowSize;
        std::cout << "Enter the column size: ";
        int kernelColumnSize;
        std::cin >> kernelColumnSize;

        if (kernelRowSize <= 0 || kernelRowSize % 2 == 0 || kernelColumnSize <= 0 || kernelColumnSize % 2 == 0) {
            std::cerr << "Kernel size must be a positive odd number." << std::endl;
            break;
        }

        std::vector<uint8_t> morphResult;

        switch (morphChoice) {
            case 1:
                std::cout << "Applying Erosion...\n";
                morphResult = applyErosion(result, kernelColumnSize, kernelRowSize);
                break;
            case 2:
                std::cout << "Applying Dilation...\n";
                morphResult = applyDilation(result, kernelColumnSize, kernelRowSize);
                break;
            case 3:
                std::cout << "Applying Opening...\n";
                morphResult = applyOpening(result, kernelColumnSize, kernelRowSize);
                break;
            case 4:
                std::cout << "Applying Closing...\n";
                morphResult = applyClosing(result, kernelColumnSize, kernelRowSize);
                break;
            case 5:
                std::cout << "Apllying Boundary Extraction...\n";
                morphResult = applyBoundaryExtraction(result, kernelColumnSize, kernelRowSize);
                break;
            case 6:
                std::cout << "Enter the seed point coordinates for the hole (row and column): ";
                int seedRow, seedCol;
                std::cin >> seedRow >> seedCol;

                if (seedRow < 0 || seedRow >= result.meta.height || seedCol < 0 || seedCol >= result.meta.width) {
                    std::cerr << "Invalid seed point. Please provide a valid point inside the image." << std::endl;
                    break;
                }

                std::cout << "Performing Hole Filing...\n";
                morphResult = applyHoleFilling(result, {seedRow, seedCol}, kernelColumnSize, kernelRowSize);
                break;
            default:
                std::cerr << "Invalid choice for morphological operation." << std::endl;
                break;
        }

        // Update the result buffer
        if (!morphResult.empty()) {
            result.buffer = morphResult;
        }

        break;
    }
    case 6: {

        std:: cout << "Seclect edge detection method: \n1. Gradient based \n2. Laplacian \n3. Canny \nChoice:";

        int method;

        std::cin >> method;

        if (method == 1) {
            int kernelChoiceInt;
            std::cout << "Select kernel:\n1. Sobel\n2. Prewitt\n3. Roberts\nChoice: ";
            std::cin >> kernelChoiceInt;

            KernelChoice kernelC = static_cast<KernelChoice>(kernelChoiceInt);

            bool thresholdYesNo;
            std::cout << "Do threshold? (0/1): ";
            std::cin >> thresholdYesNo;

            double thresholdVal = 0.0;
            if (thresholdYesNo) {
                std::cout << "Enter threshold value: ";
                std::cin >> thresholdVal;
            }

            int paddingChoiceInt;
            std::cout << "Select padding:\n0. None\n1. Zero\n2. Replicate\n3. Reflect\nChoice: ";
            std::cin >> paddingChoiceInt;

            PaddingChoice padC = static_cast<PaddingChoice>(paddingChoiceInt);      // casting the int padding choice into PaddingChoice type

            // 3. Apply edge detection
            std::vector<uint8_t> edgeBuffer = applyGradientEdgeDetection(
                result,                  // input image
                kernelC,                // kernel choice
                thresholdYesNo,         // apply threshold?
                thresholdVal,           // threshold value
                padC                    // padding choice
            );

            // Update the result buffer
            if (!edgeBuffer.empty()) {
                result.buffer = edgeBuffer;
            }

        }else if (method == 2)
        {
            std::cout << "Applying Laplacian: \n";
            
        }else if (method == 3)
        {
            std::cout << "Performing Canny Edge Detection...\n";

            int kernelSizeGaussian;

            std::cout << "Enter the kernel size for Gaussian filter: ";
            std::cin >> kernelSizeGaussian;

            int sigmaGaussian;
            std::cout << "Enter the sigma value for Gaussian filter: ";
            std::cin >> sigmaGaussian;

            int lowThreshold;
            std::cout << "Enter low threshold value: ";
            std::cin >> lowThreshold;

            int highThreshold;
            std::cout << "Enter high threshold value: ";
            std::cin >> highThreshold;

            int paddingChoiceInt;
            std::cout << "Select padding:\n0. None\n1. Zero\n2. Replicate\n3. Reflect\nChoice: ";
            std::cin >> paddingChoiceInt;

            PaddingChoice padC = static_cast<PaddingChoice>(paddingChoiceInt);      // casting the int padding choice into PaddingChoice type

            std::vector<uint8_t> edgeBuffer = applyCannyEdgeDetection(result, lowThreshold, highThreshold, sigmaGaussian, kernelSizeGaussian, padC);

            // Update the result buffer
            if (!edgeBuffer.empty()) {
                result.buffer = edgeBuffer;
            }
            
        }
        break;
    }
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
