#ifndef NUVO_IMAGE_READIMAGEPROCESS_H
#define NUVO_IMAGE_READIMAGEPROCESS_H

#include "ImageProcess.h"


class ReadImageProcess:public ImageProcess {
public:
    ReadImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, bool autoOrient)
        :ImageProcess(processor, from, to ), autoOrient(autoOrient)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "read";}
    const ProcessInputType GetFromType() { return ProcessInputType::File; }
    const ProcessInputType GetToType() { return ProcessInputType::Memory; }

private:
    bool TryReadExifOrientation(const std::vector<unsigned char> & buffer, int & orientation);
    bool TryRotateOrientation(const cv::Mat & src, cv::Mat & dest, const int orientation);
    bool autoOrient;
};

#endif
