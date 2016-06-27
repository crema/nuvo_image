//
// Created by ubuntu on 16. 6. 27.
//

#ifndef NUVO_IMAGE_OPENCV_H
#define NUVO_IMAGE_OPENCV_H

#include <opencv2/core/version.hpp>

#if CV_MAJOR_VERSION == 2
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#elif CV_MAJOR_VERSION == 3
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#endif


#endif //NUVO_IMAGE_OPENCV_H
