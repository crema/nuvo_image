#include "FrameImageProcess.h"

const ImageProcessInput FrameImageProcess::Process(const ImageProcessInput& input, picojson::object& result) {
  auto gif = input.GetGif();

  if (frame < 0 || frame >= gif.GetFrameCount()) {
    throw std::runtime_error("invalid frame");
  }

  auto image = gif.GetFrame(frame).GetMat();
  result["frame"] = picojson::value((double)frame);

  result["width"] = picojson::value((double)image.cols);
  result["height"] = picojson::value((double)image.rows);

  return ImageProcessInput(image);
}