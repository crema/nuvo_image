#ifndef NUVO_IMAGE_READIMAGEPROCESS_H
#define NUVO_IMAGE_READIMAGEPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"
#include "exif.h"

class ReadImageProcess:public ImageProcess {
public:
    ReadImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, bool autoOrient, Flatten flatten)
        :ImageProcess(processor, from, to ), autoOrient(autoOrient), flatten(flatten)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "read";}
    const int GetFromType() { return ProcessInputType::File; }
    const int GetToType() { return ProcessInputType::Image | ProcessInputType::Animation; }

private:

    bool TryFlatten(const cv::Mat & src, cv::Mat & dest);
    bool TryReadExif(const std::shared_ptr<std::vector<unsigned char>> & buffer, easyexif::EXIFInfo & exif);
    bool TryRotateOrientation(const cv::Mat & src, cv::Mat & dest, const int orientation);
    bool autoOrient;
    Flatten flatten;
};

#endif
