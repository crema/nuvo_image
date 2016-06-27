cmake_minimum_required(VERSION 3.0)

project(nuvo_image)
find_package(OpenCV 2.4 REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set (CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS '-o4')

include_directories(include)

set(SOURCE_FILES src/main.cpp
        src/exif.cpp
        src/Enums.cpp
        src/ImageProcessor.cpp
        src/ImageProcess.cpp
        src/ReadImageProcess.cpp
        src/CropImageProcess.cpp
        src/ResizeImageProcess.cpp
        src/JpegImageProcess.cpp
        src/ClearMemoryProcess.cpp)

add_executable(nuvo_image ${SOURCE_FILES})
target_link_libraries( nuvo_image ${OpenCV_LIBS} )
install (TARGETS nuvo_image DESTINATION bin)