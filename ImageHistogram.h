#ifndef IMAGE_HISTOGRAM_H
#define IMAGE_HISTOGRAM_H

#include <stdint.h>

// Function prototypes
void calculateHistogram(const uint8_t *buffer, int width, int height, int bitDepth, int *histogram, int channel);
void displayHistogram(const int *histogram, int bitDepth, bool isColor = false, char channelName = '\0');
void saveHistogramToFile(const int *histogram, int bitDepth, const std::string &fileName);
void displayHistogramAsBarChart(const int *histogram, int bitDepth);

#endif