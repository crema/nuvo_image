#ifndef NUVO_IMAGE_WEBMIMAGEPROCESS_H
#define NUVO_IMAGE_WEBMIMAGEPROCESS_H

#include "Enums.h"
#include "ImageProcess.h"

class VideoImageProcess : public InOutProcess {
 public:
  VideoImageProcess(std::shared_ptr<ImageProcessor> processor,
                    const std::string& from,
                    const std::string& to,
                    const VideoFormat format)
      : InOutProcess(processor, from, to), format(format) {}

 private:
  const ImageProcessInput Process(const ImageProcessInput& input, picojson::object& result);
  const std::string GetName() { return "video"; }
  const int GetFromType() { return ProcessInputType::Animation; }
  const int GetToType() { return ProcessInputType::File; }

  VideoFormat format;
};

#endif  // NUVO_IMAGE_WEBMIMAGEPROCESS_H
