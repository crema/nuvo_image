#ifndef NUVO_IMAGE_READIMAGEPROCESS_H
#define NUVO_IMAGE_READIMAGEPROCESS_H

#include "opencv.h"
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
    const ProcessInputType GetFromType() { return ProcessInputType::File; }
    const ProcessInputType GetToType() { return ProcessInputType::Memory; }

private:
    bool TryFlatten(const cv::Mat & src, cv::Mat & dest);
    bool TryReadExif(const std::vector<unsigned char> & buffer, easyexif::EXIFInfo & exif);
    bool TryRotateOrientation(const cv::Mat & src, cv::Mat & dest, const int orientation);
    bool autoOrient;
    Flatten flatten;
};

#endif
