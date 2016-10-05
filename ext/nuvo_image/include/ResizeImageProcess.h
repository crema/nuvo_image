#ifndef NUVO_IMAGE_RESIZEIMAGEPROCESS_H
#define NUVO_IMAGE_RESIZEIMAGEPROCESS_H

#include "OpenCV.h"
#include "Enums.h"
#include "ImageProcess.h"

class ResizeImageProcess: public InOutProcess{
public:
    ResizeImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, int width, int height, Interpolation interpolation)
        :InOutProcess(processor, from, to), width(width), height(height), interpolation(interpolation)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "resize";}
    const int GetFromType() { return ProcessInputType::Image | ProcessInputType::Animation; }
    const int GetToType() { return ProcessInputType::Image | ProcessInputType::Animation; }
private:

    const cv::Mat ResizeMat(const cv::Mat &mat);

    int width;
    int height;
    Interpolation interpolation;
};


#endif //NUVO_IMAGE_RESIZEIMAGEPROCESS_H
