cmake_minimum_required(VERSION 3.0)

project(nuvo_image)


if(UNIX AND NOT APPLE)
    set (CMAKE_CXX_COMPILER g++-6)
endif()

set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_FLAGS '-o4')

include(ExternalProject)

set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
set(EXTERNAL_SRC_DIR ${EXTERNAL_DIR}/src)
set(EXTERNAL_LIB_DIR ${EXTERNAL_DIR}/lib)

ExternalProject_Add(
        giflib
        URL http://downloads.sourceforge.net/project/giflib/giflib-5.1.4.tar.bz2
        SOURCE_DIR ${EXTERNAL_SRC_DIR}/giflib-5.1.4
        CONFIGURE_COMMAND ${EXTERNAL_SRC_DIR}/giflib-5.1.4/configure --prefix=${EXTERNAL_LIB_DIR}/giflib-5.1.4
        BUILD_IN_SOURCE 1
)

ExternalProject_Add(
        mozjpeg
        URL https://github.com/mozilla/mozjpeg/releases/download/v3.1/mozjpeg-3.1-release-source.tar.gz
        SOURCE_DIR ${EXTERNAL_SRC_DIR}/mozjpeg
        CONFIGURE_COMMAND ${EXTERNAL_SRC_DIR}/mozjpeg/configure --prefix=${EXTERNAL_LIB_DIR}/mozjpeg
        BUILD_IN_SOURCE 1
)

set(FFMPEG_CONFIGURE
        --disable-bzlib
        --enable-nonfree
        --enable-pic
        --disable-doc
        --enable-gpl
        --enable-avresample
        --enable-libvpx
        --enable-libx264)


set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-decoders)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-encoders
        --enable-encoder=libvpx_vp8
        --enable-encoder=libx264)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-parsers
        --enable-parser=h264
        --enable-parser=vp8)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-demuxers)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-muxers
        --enable-muxer=h264
        --enable-muxer=webm
        --enable-muxer=mp4)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-protocols
        --enable-protocol=file)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-filters)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-bsfs)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-indevs)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --disable-outdevs)

set(FFMPEG_CONFIGURE ${FFMPEG_CONFIGURE}
        --prefix=${EXTERNAL_LIB_DIR}/ffmpeg-3.1.1)

ExternalProject_Add(
        ffmpeg
        URL http://ffmpeg.org/releases/ffmpeg-3.1.1.tar.bz2
        SOURCE_DIR ${EXTERNAL_SRC_DIR}/ffmpeg-3.1.1
        CONFIGURE_COMMAND ${EXTERNAL_SRC_DIR}/ffmpeg-3.1.1/configure ${FFMPEG_CONFIGURE}
        BUILD_IN_SOURCE 1
)

ExternalProject_Add(
        opencv
        URL https://github.com/Itseez/opencv/archive/3.1.0.zip
        SOURCE_DIR ${EXTERNAL_SRC_DIR}/opencv3.1.0
        CMAKE_ARGS -C${EXTERNAL_DIR}/opencv_config.gch
        BUILD_IN_SOURCE 1
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin)


include_directories(include
        ${OpenCV_INCLUDE_DIR}
        ${EXTERNAL_LIB_DIR}/giflib-5.1.4/include
        ${EXTERNAL_LIB_DIR}/mozjpeg/include
        ${EXTERNAL_LIB_DIR}/opencv3.1.0/include)

set(SOURCE_FILES
        ${GIF_SRC}
        src/main.cpp
        src/exif.cpp
        src/Gif.cpp
        src/Enums.cpp
        src/ImageProcessor.cpp
        src/ImageProcess.cpp
        src/ReadImageProcess.cpp
        src/CropImageProcess.cpp
        src/ResizeImageProcess.cpp
        src/LossyImageProcess.cpp
        src/FrameImageProcess.cpp
        src/VideoImageProcess.cpp
        src/ClearMemoryProcess.cpp)

add_executable(nuvo_image ${SOURCE_FILES})

add_dependencies(opencv mozjpeg ffmpeg)
add_dependencies(nuvo_image giflib opencv)

find_library(GIF_LIBRARIES NAMES libgif.a
        HINTS ${EXTERNAL_LIB_DIR}/giflib-5.1.4/lib)
find_library(MOZJPEG_LIBRARIES jpeg
        HINTS ${EXTERNAL_LIB_DIR}/mozjpeg/lib)
find_library(OPENCV_CORE_LIBRARIES NAMES opencv_core
        HINTS ${EXTERNAL_LIB_DIR}/opencv3.1.0/lib)
find_library(OPENCV_IMGPROC_LIBRARIES NAMES opencv_imgproc
        HINTS ${EXTERNAL_LIB_DIR}/opencv3.1.0/lib)
find_library(OPENCV_IMGCODECS_LIBRARIES NAMES opencv_imgcodecs
        HINTS ${EXTERNAL_LIB_DIR}/opencv3.1.0/lib)
find_library(OPENCV_VIDEOIO_LIBRARIES NAMES opencv_videoio
        HINTS ${EXTERNAL_LIB_DIR}/opencv3.1.0/lib)

find_library(X264_LIBRARIES x264)
find_library(VPX_LIBRARIES vpx)

target_link_libraries(nuvo_image
        ${GIF_LIBRARIES}
        ${MOZJPEG_LIBRARIES}
        ${OPENCV_CORE_LIBRARIES}
        ${OPENCV_IMGPROC_LIBRARIES}
        ${OPENCV_IMGCODECS_LIBRARIES}
        ${OPENCV_VIDEOIO_LIBRARIES}
        ${X264_LIBRARIES}
        ${VPX_LIBRARIES})

install (TARGETS nuvo_image DESTINATION bin)