cmake_minimum_required(VERSION 3.16)

project(ImageProcessing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ImageProcessing
    src/main.cpp
    src/ImageIO.cpp
    src/IntensityTransformations.cpp
    src/ImageHistogram.cpp
    src/ImageFilter.cpp
    src/ImageConverter.cpp
    src/ImageMorphology.cpp 
    src/ImageEdgeDetection.cpp
    src/ImageUtils.cpp 
)

# Include directories for headers
target_include_directories(ImageProcessing 
    PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
