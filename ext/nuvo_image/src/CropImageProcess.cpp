#include "CropImageProcess.h"
#include <cmath>

const ImageProcessInput CropImageProcess::Process(const ImageProcessInput &input, picojson::object &result) {

    auto inputWidth = input.GetWidth();
    auto inputHeight = input.GetHeight();

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

    result["gravity"] = picojson::value(ToString(gravity));
    result["top"] = picojson::value((double)outputTop);
    result["left"] = picojson::value((double)outputLeft);
    result["width"] = picojson::value((double)outputWidth);
    result["height"] = picojson::value((double)outputHeight);


    auto rect = cv::Rect(outputLeft, outputTop, outputWidth, outputHeight);
    if(!input.GetGif().Empty()){
        const auto gif = input.GetGif();
        Gif cropped;

        for(int i = 0; i < gif.GetFrameCount(); ++i){
            auto frame = gif.GetFrame(i);
            cropped.AddFrame(Gif::GifFrame(frame.GetMat()(rect), frame.GetDelay()));
        }

        result["frames"] = picojson::value((double)gif.GetFrameCount());
        return ImageProcessInput(cropped);
    } else {
        return ImageProcessInput(input.GetMat()(rect));
    }
}