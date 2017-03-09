#include "ResizeImageProcess.h"
#include "ImageProcessor.h"
#include "Enums.h"

const ImageProcessInput ResizeImageProcess::Process(const ImageProcessInput& input, picojson::object& result) {
  result["width"] = picojson::value((double)width);
  result["height"] = picojson::value((double)height);
  result["interpolation"] = picojson::value(ToString(interpolation));

  if (!input.GetGif().Empty()) {
    const auto gif = input.GetGif();
    Gif resized;

    for (int i = 0; i < gif.GetFrameCount(); ++i) {
      auto frame = gif.GetFrame(i);
      resized.AddFrame(GifFrame(ResizeMat(frame.GetMat()), frame.GetDelay()));
    }
    result["frames"] = picojson::value((double)gif.GetFrameCount());
    return ImageProcessInput(resized);
  } else {
    return ResizeMat(input.GetMat());
  }
}

const cv::Mat ResizeImageProcess::ResizeMat(const cv::Mat& mat) {
  cv::Mat resized;
  cv::resize(mat, resized, cv::Size(width, height), 0, 0, interpolation);
  return resized;
}