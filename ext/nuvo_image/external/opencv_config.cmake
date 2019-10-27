set(CMAKE_BUILD_TYPE Release CACHE STRING "")

set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/../../lib)

set(WITH_JPEG ON CACHE BOOL "")
set(BUILD_JPEG OFF CACHE BOOL "")
if (UNIX AND NOT APPLE)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,-Bsymbolic" CACHE STRING "")
        set(JPEG_INCLUDE_DIR ${EXTERNAL_LIB_DIR}/mozjpeg-3.1/include CACHE PATH "")
        set(JPEG_LIBRARY ${EXTERNAL_LIB_DIR}/mozjpeg-3.1/lib/libjpeg.a CACHE FILEPATH "")
else ()
        set(JPEG_INCLUDE_DIR /usr/local/opt/mozjpeg/include CACHE PATH "")
        set(JPEG_LIBRARY /usr/local/opt/mozjpeg/lib/libjpeg.a CACHE FILEPATH "")
endif ()

set(WITH_FFMPEG ON CACHE BOOL "")
set(ENV{PKG_CONFIG_PATH} "${EXTERNAL_LIB_DIR}/ffmpeg-4.2.1/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

set(CMAKE_INSTALL_PREFIX ${EXTERNAL_LIB_DIR}/opencv-3.4.7 CACHE PATH "")

set(BUILD_DOCS OFF CACHE BOOL "")
set(BUILD_PERF_TESTS OFF CACHE BOOL "")
set(BUILD_TESTS OFF CACHE BOOL "")

set(BUILD_opencv_apps OFF CACHE BOOL "")
set(BUILD_opencv_calib3d OFF CACHE BOOL "")
set(BUILD_opencv_features2d OFF CACHE BOOL "")
set(BUILD_opencv_flann OFF CACHE BOOL "")
set(BUILD_opencv_highgui OFF CACHE BOOL "")
set(BUILD_opencv_java OFF CACHE BOOL "")
set(BUILD_opencv_ml OFF CACHE BOOL "")
set(BUILD_opencv_objdetect OFF CACHE BOOL "")
set(BUILD_opencv_photo OFF CACHE BOOL "")
set(BUILD_opencv_python2 OFF CACHE BOOL "")
set(BUILD_opencv_python3 OFF CACHE BOOL "")
set(BUILD_opencv_shape OFF CACHE BOOL "")
set(BUILD_opencv_stitching OFF CACHE BOOL "")
set(BUILD_opencv_superres OFF CACHE BOOL "")
set(BUILD_opencv_ts OFF CACHE BOOL "")
set(BUILD_opencv_video OFF CACHE BOOL "")
set(BUILD_opencv_videostab OFF CACHE BOOL "")
set(BUILD_opencv_viz OFF CACHE BOOL "")

set(WITH_CUDA OFF CACHE BOOL "")
set(WITH_GPHOTO2 OFF CACHE BOOL "")
set(WITH_GSTREAMER OFF CACHE BOOL "")
set(WITH_GTK OFF CACHE BOOL "")
set(WITH_IPP OFF CACHE BOOL "")
set(WITH_OPENEXR OFF CACHE BOOL "")
set(WITH_QT OFF CACHE BOOL "")

set(BUILD_JASPER ON CACHE BOOL "")
set(BUILD_PNG ON CACHE BOOL "")
set(BUILD_TIFF ON CACHE BOOL "")
