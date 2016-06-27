#include <opencv2/imgproc.hpp>
#include "ResizeImageProcess.h"
#include "ImageProcessor.h"
#include "Enums.h"

const ImageProcessInput ResizeImageProcess::Process(const ImageProcessInput &input, picojson::object &result) {
    const auto image = input.GetMat();
    cv::Mat resized;
    cv::resize(image, resized, cv::Size(width, height), 0, 0, interpolation);

    result["width"] = picojson::value((double)width);
    result["height"] = picojson::value((double)height);
    result["interpolation"] = picojson::value(ToString(interpolation));
    return input;
}