#ifndef NUVO_IMAGE_LOSSLESSIMAGEPROCESS_H
#define NUVO_IMAGE_LOSSLESSIMAGEPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"
#include "Enums.h"

class LosslessImageProcess : public InOutProcess {
 public:
  LosslessImageProcess(std::shared_ptr<ImageProcessor> processor,
                       const std::string& from,
                       const std::string& to,
                       const LosslessImageFormat format)
      : InOutProcess(processor, from, to), format(format) {}

 private:
  const ImageProcessInput Process(const ImageProcessInput& input, picojson::object& result);
  const std::string GetName() { return "lossless"; }
  const int GetFromType() { return ProcessInputType::Image | ProcessInputType::Animation; }
  const int GetToType() { return ProcessInputType::File; }
  void Encode(const std::shared_ptr<std::vector<unsigned char>>& buffer, const cv::Mat& image);

  LosslessImageFormat format;
};

#endif  // NUVO_IMAGE_LOSSLESSIMAGEPROCESS_H
