cmake_minimum_required(VERSION 3.16)

project(ImageProcessing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ImageProcessing
    main.cpp
    ImageIO.cpp
    IntensityTransformations.cpp
    ImageHistogram.cpp
    ImageFilter.cpp
    ImageConverter.cpp
    ImageMorphology.cpp 
)

# Include directories for headers
target_include_directories(ImageProcessing PRIVATE ${CMAKE_SOURCE_DIR})
