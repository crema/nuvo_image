
find_library(X264_LIBRARIES x264)
find_library(VPX_LIBRARIES vpx)

set(VIDEOIO_LIBRARIES ${VIDEOIO_LIBRARIES}
        ${X264_LIBRARIES}
        ${VPX_LIBRARIES})

if(APPLE)
    find_library(COREMEDIA CoreMedia)
    find_library(COREVIDEO CoreVideo)

    set(VIDEOIO_LIBRARIES ${VIDEOIO_LIBRARIES}
            ${COREMEDIA}
            ${COREVIDEO})
endif()