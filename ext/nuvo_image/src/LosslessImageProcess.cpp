#include "LosslessImageProcess.h"

const ImageProcessInput LosslessImageProcess::Process(const ImageProcessInput& input, picojson::object& result) {
  auto buffer = std::make_shared<std::vector<unsigned char>>();

  if (input.GetType() == ProcessInputType::Image) {
    Encode(buffer, input.GetMat());
  } else if (input.GetType() == ProcessInputType::Animation) {
    Encode(buffer, input.GetGif().GetFrame(0).GetMat());
  } else {
    throw std::runtime_error("invalid lossless input");
  }

  result["format"] = picojson::value(ToString(format));
  return ImageProcessInput(buffer);
}

void LosslessImageProcess::Encode(const std::shared_ptr<std::vector<unsigned char>>& buffer, const cv::Mat& image) {
  buffer->clear();
  std::vector<int> imageParams;
  if (format == LosslessImageFormat::Png) {
    cv::imencode(".png", image, *buffer, imageParams);
  } else if (format == LosslessImageFormat::LosslessWebP) {
    imageParams.push_back(cv::IMWRITE_WEBP_QUALITY);
    imageParams.push_back(100);
    cv::imencode(".webp", image, *buffer, imageParams);
  } else {
    throw std::runtime_error("invalid image format");
  }
}
