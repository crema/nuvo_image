#include "CropImageProcess.h"
#include <cmath>

const ImageProcessInput CropImageProcess::Process(const ImageProcessInput &input, picojson::object &result) {
    auto image = input.GetMat();

    auto inputWidth = image.cols;
    auto inputHeight = image.rows;

    auto outputWidth = inputWidth < width ? inputWidth: width;
    auto outputHeight = inputHeight < height ? inputHeight: height;

    int outputTop, outputLeft;

    if(IsNorth()) {
        outputTop = 0;
    } else if(IsSouth()) {
        outputTop = inputHeight - outputHeight;
    } else {
        outputTop = (int)std::round((inputHeight - outputHeight)/2.0);
    }

    if(IsWest()) {
        outputLeft = 0;
    } else if(IsEast()) {
        outputLeft = inputWidth - outputWidth;
    } else {
        outputLeft = (int)std::round((inputWidth - outputWidth)/2.0);
    }

    auto croped = image(cv::Rect(outputLeft, outputTop, outputWidth, outputHeight));

    result["gravity"] = picojson::value(ToString(gravity));
    result["top"] = picojson::value((double)outputTop);
    result["left"] = picojson::value((double)outputLeft);
    result["width"] = picojson::value((double)outputWidth);
    result["height"] = picojson::value((double)outputHeight);

    return ImageProcessInput(croped);
}