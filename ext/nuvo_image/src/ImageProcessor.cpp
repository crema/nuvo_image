#include "ImageProcessor.h"
#include "ReadImageProcess.h"
#include "CropImageProcess.h"
#include "ResizeImageProcess.h"
#include "LossyImageProcess.h"
#include "LosslessImageProcess.h"
#include "FrameImageProcess.h"
#include "VideoImageProcess.h"
#include "ClearMemoryProcess.h"
#include "CloseProcess.h"
#include "CompareImageProcess.h"

std::shared_ptr<ImageProcess> ImageProcessor::Parse(const std::string& inputString) {
  picojson::value json;
  auto error = picojson::parse(json, inputString);

  if (!error.empty())
    throw std::runtime_error(error);

  picojson::object object;
  if (!TryGet(json, object)) {
    throw std::runtime_error("invalid json");
  }

  auto processString = GetMember<std::string>(object, "process");
  auto from = GetMember<std::string>(object, "from", "");
  auto to = GetMember<std::string>(object, "to", "");

  if (processString == "read") {
    auto autoOrient = GetMember<bool>(object, "auto_orient", false);
    auto flatten = GetMember<Flatten>(object, "flatten", Flatten::None);

    return std::shared_ptr<ImageProcess>(new ReadImageProcess(shared_from_this(), from, to, autoOrient, flatten));
  } else if (processString == "crop") {
    auto width = (int)GetMember<double>(object, "width");
    auto height = (int)GetMember<double>(object, "height");
    auto gravity = GetMember<Gravity>(object, "gravity", Gravity::Center);

    return std::shared_ptr<ImageProcess>(new CropImageProcess(shared_from_this(), from, to, width, height, gravity));
  } else if (processString == "resize") {
    auto width = (int)GetMember<double>(object, "width");
    auto height = (int)GetMember<double>(object, "height");
    auto interpolation = GetMember<Interpolation>(object, "interpolation", Interpolation::Area);

    return std::shared_ptr<ImageProcess>(
        new ResizeImageProcess(shared_from_this(), from, to, width, height, interpolation));
  } else if (processString == "lossy") {
    auto format = GetMember<LossyImageFormat>(object, "format", LossyImageFormat::Jpeg);
    auto quality = GetMember<ImageQuality>(object, "quality", ImageQuality(95));
    auto min = (int)GetMember<double>(object, "min", 55);
    auto max = (int)GetMember<double>(object, "max", 95);
    auto search = (int)GetMember<double>(object, "search", 5);

    return std::shared_ptr<ImageProcess>(
        new LossyImageProcess(shared_from_this(), from, to, format, quality, min, max, search));
  } else if (processString == "lossless") {
    auto format = GetMember<LosslessImageFormat>(object, "format", LosslessImageFormat::Png);
    return std::shared_ptr<ImageProcess>(new LosslessImageProcess(shared_from_this(), from, to, format));
  } else if (processString == "frame") {
    auto frame = (int)GetMember<double>(object, "frame", 0);

    return std::shared_ptr<ImageProcess>(new FrameImageProcess(shared_from_this(), from, to, frame));
  } else if (processString == "compare") {
    auto from1 = GetMember<std::string>(object, "from1", "");
    auto from2 = GetMember<std::string>(object, "from2", "");

    return std::shared_ptr<ImageProcess>(new CompareImageProcess(shared_from_this(), from1, from2));
  } else if (processString == "video") {
    auto format = GetMember<VideoFormat>(object, "format", VideoFormat::Mp4);

    return std::shared_ptr<ImageProcess>(new VideoImageProcess(shared_from_this(), from, to, format));
  } else if (processString == "clear") {
    return std::shared_ptr<ImageProcess>(new ClearMemoryProcess(shared_from_this()));
  } else if (processString == "close") {
    return std::shared_ptr<ImageProcess>(new CloseProcess(shared_from_this()));
  }

  throw std::runtime_error("invalid input");
}

bool ImageProcessor::TryGet(const std::string& name, ImageProcessInput& input) {
  auto findData = data.find(name);

  if (findData == data.end())
    return false;

  input = findData->second;
  return true;
}

void ImageProcessor::Insert(const std::string& name, const ImageProcessInput& input) {
  data.insert(std::make_pair(name, input));
}

void ImageProcessor::Clear() {
  data.clear();
}
