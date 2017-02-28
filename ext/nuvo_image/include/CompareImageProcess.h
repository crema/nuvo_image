//
// Created by ubuntu on 16. 8. 25.
//

#ifndef NUVO_IMAGE_COMPAREIMAGEPROCESS_H
#define NUVO_IMAGE_COMPAREIMAGEPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"

class CompareImageProcess : public TwoImageToNullProcess {
 public:
  CompareImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string& from1, const std::string& from2)
      : TwoImageToNullProcess(processor, from1, from2) {}

 protected:
  void Process(const ImageProcessInput& input1, const ImageProcessInput& input2, picojson::object& result);
  const std::string GetName() { return "compare"; }
};

#endif  // NUVO_IMAGE_COMPAREIMAGEPROCESS_H
