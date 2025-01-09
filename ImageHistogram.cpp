#include "ImageHistogram.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>


void calculateHistogram(const ImageReadResult &result, std::vector<int> &histogram, int channel) {

    const ImageMetadata &meta = result.meta;
    const uint8_t *buffer = result.buffer->data();

    int numBins = (meta.bitDepth <= 8) ? NUM_BINS_8BIT : NUM_BINS_16BIT;
    histogram.assign(numBins, 0); // Reset the histogram

    int stride = (channel == -1) ? 1 : 3;
    int totalPixels = meta.width * meta.height;

    for (int i = (channel == -1 ? 0 : channel); i < totalPixels * stride; i += stride) {
        if (meta.bitDepth == 8) {
            histogram[buffer[i]]++;
        } else if (meta.bitDepth == 16) {
            const uint16_t *buffer16 = reinterpret_cast<const uint16_t *>(buffer);
            histogram[buffer16[i]]++;
        }
    }
}

void displayHistogram(const std::vector<int> &histogram, int bitDepth, bool isColor, char channelName) {
    if (isColor) {
        std::cout << "Histogram for channel: " << channelName << "\n";
    } else {
        std::cout << "Histogram for grayscale image\n";
    }

    std::cout << "Intensity Level\tFrequency\n";
    for (size_t i = 0; i < histogram.size(); ++i) {
        if (histogram[i] > 0) {
            std::cout << i << "\t\t" << histogram[i] << "\n";
        }
    }
}

void saveHistogramToFile(const std::vector<int> &histogram, const std::string &fileName) {
    std::ofstream outFile(fileName);
    if (!outFile) {
        std::cerr << "Error: Unable to open file for writing: " << fileName << "\n";
        return;
    }

    outFile << "Intensity Level, Frequency\n";
    for (size_t i = 0; i < histogram.size(); ++i) {
        if (histogram[i] > 0) {
            outFile << i << ", " << histogram[i] << "\n";
        }
    }

    std::cout << "Histogram data saved to " << fileName << "\n";
}

void displayHistogramAsBarChart(const std::vector<int> &histogram) {
    int maxFrequency = *std::max_element(histogram.begin(), histogram.end());

    std::cout << "Histogram (Bar Chart):\n";
    for (size_t i = 0; i < histogram.size(); ++i) {
        if (histogram[i] > 0) {
            std::cout << i << ": ";
            int barLength = (50 * histogram[i]) / maxFrequency; // Scale to 50 characters
            std::cout << std::string(barLength, '#') << " (" << histogram[i] << ")\n";
        }
    }
}

void grayscaleHistogram(const ImageReadResult &result) {
    std::vector<int> histogram(NUM_BINS_8BIT);
    calculateHistogram(result, histogram);
    displayHistogram(histogram, result.meta.bitDepth, false, 'N');
    saveHistogramToFile(histogram, "histogram_data.csv");
    displayHistogramAsBarChart(histogram);
}

void rgbHistogram(const ImageReadResult &result) {
    if (result.meta.bitDepth != 24) {
        std::cerr << "RGB histogram is only supported for 24-bit images.\n";
        return;
    }

    std::vector<int> redHistogram(NUM_BINS_8BIT);
    std::vector<int> greenHistogram(NUM_BINS_8BIT);
    std::vector<int> blueHistogram(NUM_BINS_8BIT);

    calculateHistogram(result, redHistogram, 0);
    calculateHistogram(result, greenHistogram, 1);
    calculateHistogram(result, blueHistogram, 2);

    displayHistogram(redHistogram, 8, true, 'R');
    displayHistogram(greenHistogram, 8, true, 'G');
    displayHistogram(blueHistogram, 8, true, 'B');

    saveHistogramToFile(redHistogram, "redHistogram_data.csv");
    saveHistogramToFile(greenHistogram, "greenHistogram_data.csv");
    saveHistogramToFile(blueHistogram, "blueHistogram_data.csv");

    displayHistogramAsBarChart(redHistogram);
    displayHistogramAsBarChart(greenHistogram);
    displayHistogramAsBarChart(blueHistogram);
}
