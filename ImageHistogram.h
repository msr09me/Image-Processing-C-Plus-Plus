#ifndef IMAGE_HISTOGRAM_H
#define IMAGE_HISTOGRAM_H

#include <vector>
#include <string>
#include <cstdint>
#include "ImageIO.h"

constexpr int NUM_BINS_8BIT = 256;
constexpr int NUM_BINS_16BIT = 65536;
constexpr char DEFAULT_HISTOGRAM_FILE[] = "histogram_data.csv";


// Function prototypes
void calculateHistogram(const ImageReadResult &result, std::vector<int> &histogram, int channel = -1) ;
void displayHistogram(const std::vector<int> &histogram, int bitDepth, bool isColor, char channelName) ;
void saveHistogramToFile(const std::vector<int> &histogram, const std::string &fileName);
void displayHistogramAsBarChart(const std::vector<int> &histogram) ;
void grayscaleHistogram(const ImageReadResult &result) ;
void rgbHistogram(const ImageReadResult &result);
std::vector<uint8_t> histogramEqualization(const ImageReadResult &result);

/**
 * Performs histogram equalization on a grayscale image.
 *
 * @param result The image data and metadata.
 * @return A new buffer with the equalized grayscale image.
 */
std::vector<uint8_t> histogramEqualization(const ImageReadResult &result);

#endif