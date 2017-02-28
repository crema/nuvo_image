#ifndef NUVO_IMAGE_EXITPROCESS_H
#define NUVO_IMAGE_EXITPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"

class CloseProcess : public NullToNullProcess {
 public:
  CloseProcess(std::shared_ptr<ImageProcessor> processor) : NullToNullProcess(processor) {}

 protected:
  void Process(picojson::object& result) { exit(0); }
  const std::string GetName() { return "close"; }
};

#endif  // NUVO_IMAGE_EXITPROCESS_H
