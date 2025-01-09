#include <iostream>
#include <fstream>
#include "ImageHistogram.h"


// Function to calculate the histogram for grayscale and RGB images
void calculateHistogram(const uint8_t *buffer, int width, int height, int bitDepth, int *histogram, int channel = -1) {
    // Initialize the histogram array
    int numBins = (bitDepth <= 8) ? 256 : 65536; // 8-bit = 256 bins, 16-bit = 65536 bins
    for (int i = 0; i < numBins; ++i) {
        histogram[i] = 0;
    }

    // For grayscale (single channel)
    if (channel == -1) {
        int totalPixels = width * height;
        if (bitDepth == 8) {
            for (int i = 0; i < totalPixels; ++i) {
                histogram[buffer[i]]++;
            }
        } else if (bitDepth == 16) {
            const uint16_t *buffer16 = reinterpret_cast<const uint16_t *>(buffer);
            for (int i = 0; i < totalPixels; ++i) {
                histogram[buffer16[i]]++;
            }
        }
    }
    // For RGB (separate channels)
    else {
        if (bitDepth == 8) {
            for (int i = channel; i < width * height * 3; i += 3) {
                histogram[buffer[i]]++;
            }
        } else if (bitDepth == 16) {
            const uint16_t *buffer16 = reinterpret_cast<const uint16_t *>(buffer);
            for (int i = channel; i < width * height * 3; i += 3) {
                histogram[buffer16[i]]++;
            }
        }
    }
}

// Function to display the histogram
void displayHistogram(const int *histogram, int bitDepth, bool isColor, char channelName) {
    int numBins = (bitDepth <= 8) ? 256 : 65536;

    if (isColor) {
        std::cout << "Histogram for channel: " << channelName << "\n";
    } else {
        std::cout << "Histogram for grayscale image\n";
    }

    std::cout << "Intensity Level\tFrequency" << std::endl;
    for (int i = 0; i < numBins; ++i) {
        if (histogram[i] > 0) { // Display non-zero bins for readability
            std::cout << i << "\t\t" << histogram[i] << std::endl;
        }
    }
}


// Function to save histogram data to a file
void saveHistogramToFile(const int *histogram, int bitDepth, const std::string &fileName) {
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << fileName << std::endl;
        return;
    }

    int numBins = (bitDepth <= 8) ? 256 : 65536; // Determine number of bins based on bit depth

    outFile << "Intensity Level, Frequency\n";
    for (int i = 0; i < numBins; ++i) {
        if (histogram[i] > 0) { // Save non-zero values only
            outFile << i << ", " << histogram[i] << "\n";
        }
    }

    std::cout << "Histogram data saved to " << fileName << std::endl;
    outFile.close();
}

void displayHistogramAsBarChart(const int *histogram, int bitDepth) {
    int numBins = (bitDepth <= 8) ? 256 : 65536;
    int maxFrequency = 0;

    // Find the maximum frequency to normalize the bars
    for (int i = 0; i < numBins; ++i) {
        if (histogram[i] > maxFrequency) {
            maxFrequency = histogram[i];
        }
    }

    std::cout << "Histogram (Bar Chart):\n";
    for (int i = 0; i < numBins; ++i) {
        if (histogram[i] > 0) {
            std::cout << i << ": ";
            int barLength = (50 * histogram[i]) / maxFrequency; // Scale to 50 characters
            for (int j = 0; j < barLength; ++j) {
                std::cout << "#";
            }
            std::cout << " (" << histogram[i] << ")\n";
        }
    }
}


void grayscaleHistogram(const ImageReadResult &result) {
    int histogram[65536] = {0};
    calculateHistogram(result.buffer->data(), result.meta.width, result.meta.height, result.meta.bitDepth, histogram, -1);
    displayHistogram(histogram, result.meta.bitDepth, false, 'N');

    std::string outputFileName = "histogram_data.csv";
    saveHistogramToFile(histogram, result.meta.bitDepth, outputFileName);

    displayHistogramAsBarChart(histogram, result.meta.bitDepth);
}

void rGBHistogram(const ImageReadResult &result) {
    if (result.meta.bitDepth != 24) {
        std::cerr << "RGB histogram is only supported for 24-bit images." << std::endl;
        return;
    }

    int redHistogram[256] = {0};
    int greenHistogram[256] = {0};
    int blueHistogram[256] = {0};

    calculateHistogram(result.buffer->data(), result.meta.width, result.meta.height, 8, redHistogram, 0);   // Red channel
    calculateHistogram(result.buffer->data(), result.meta.width, result.meta.height, 8, greenHistogram, 1); // Green channel
    calculateHistogram(result.buffer->data(), result.meta.width, result.meta.height, 8, blueHistogram, 2);  // Blue channel

    displayHistogram(redHistogram, 8, true, 'R');
    displayHistogram(greenHistogram, 8, true, 'G');
    displayHistogram(blueHistogram, 8, true, 'B');

    saveHistogramToFile(redHistogram, 8, "redHistogram_data.csv");
    saveHistogramToFile(greenHistogram, 8, "greenHistogram_data.csv");
    saveHistogramToFile(blueHistogram, 8, "blueHistogram_data.csv");

    displayHistogramAsBarChart(redHistogram, 8);
    displayHistogramAsBarChart(greenHistogram, 8);
    displayHistogramAsBarChart(blueHistogram, 8);
}
