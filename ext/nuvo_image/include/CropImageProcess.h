#ifndef NUVO_IMAGE_CROPIMAGEPROCESS_H
#define NUVO_IMAGE_CROPIMAGEPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"
#include "Enums.h"

class CropImageProcess : public InOutProcess {
 public:
  CropImageProcess(std::shared_ptr<ImageProcessor> processor,
                   const std::string& from,
                   const std::string& to,
                   int width,
                   int heigh,
                   Gravity gravity)
      : InOutProcess(processor, from, to), width(width), height(heigh), gravity(gravity) {}

 protected:
  const ImageProcessInput Process(const ImageProcessInput& input, picojson::object& result);
  const std::string GetName() { return "crop"; }
  const int GetFromType() { return ProcessInputType::Image | ProcessInputType::Animation; }
  const int GetToType() { return ProcessInputType::Image | ProcessInputType::Animation; }

 private:
  bool IsWest() { return gravity & West; }
  bool IsEast() { return gravity & East; }

  bool IsNorth() { return gravity & North; }
  bool IsSouth() { return gravity & South; }

  bool IsCenter() { return gravity == Center; }

  int width;
  int height;
  Gravity gravity;
};

#endif  // NUVO_IMAGE_CROPIMAGEPROCESS_H
