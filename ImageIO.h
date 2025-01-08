#ifndef IMAGE_IO_H
#define IMAGE_IO_H

#include <string>
#include <vector>
#include <optional>
#include <cstdint>
#include <iostream>

// Constants
constexpr size_t HEADER_SIZE = 54;              // Standard BMP header size
constexpr size_t COLOR_TABLE_SIZE = 1024;       // Maximum size of the color table for BMP

// Image Metadata Structure
struct ImageMetadata {
    int width = 0;       // Image width in pixels
    int height = 0;      // Image height in pixels
    int bitDepth = 0;    // Bits per pixel (e.g., 8, 24, etc.)

    // Constructor for easy initialization
    ImageMetadata(int w = 0, int h = 0, int depth = 0)
        : width(w), height(h), bitDepth(depth) {}

    // Helper function to validate metadata
    bool isValid() const {
        return width > 0 && height > 0 && bitDepth > 0;
    }
};

struct ImageReadResult {
    std::optional<std::vector<uint8_t>> buffer; // Pixel data buffer
    std::vector<uint8_t> colorTable;            // Color table
    std::vector<uint8_t> header;               // BMP header
};

// Log Levels for Debugging
enum LogLevel { INFO, WARNING, ERROR };

// Function Prototypes

/**
 * Reads an image file into a buffer.
 *
 * @param filePath Path to the input image file.
 * @param meta Reference to ImageMetadata to store extracted metadata.
 * @return A buffer containing the image pixel data, or std::nullopt on failure.
 */
ImageReadResult readImage(const std::string &filePath, ImageMetadata &meta);

/**
 * Writes an image to a file.
 *
 * @param filePath Path to the output image file.
 * @param meta ImageMetadata containing dimensions and bit depth.
 * @param header Pointer to the BMP header data.
 * @param colorTable Pointer to the BMP color table data (if applicable).
 * @param buffer The image pixel data to write.
 * @return True if the write was successful, false otherwise.
 */
bool writeImage(const std::string &filePath, const ImageMetadata &meta, 
                std::vector<uint8_t> header, const std::vector<uint8_t> &colorTable, 
                const std::vector<uint8_t> &buffer) ;

/**
 * Detects the format of an image file based on its signature.
 *
 * @param filePath Path to the image file.
 * @return A string representing the detected file format (e.g., "BMP"), or "unknown" if detection fails.
 */
std::string detectFileFormat(const std::string &filePath);

/**
 * Logs messages with specified log levels.
 *
 * @param level The log level (INFO, WARNING, or ERROR).
 * @param message The message to log.
 */
void log(LogLevel level, const std::string &message);

#endif // IMAGE_IO_H
