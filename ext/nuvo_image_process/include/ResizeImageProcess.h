#ifndef NUVO_IMAGE_PROCESS_RESIZEIMAGEPROCESS_H
#define NUVO_IMAGE_PROCESS_RESIZEIMAGEPROCESS_H

#include <opencv2/imgproc.hpp>
#include "Enums.h"
#include "ImageProcess.h"

bool TryParse(const std::string & str, Interpolation & value);

class ResizeImageProcess: public ImageProcess{
public:
    ResizeImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, int width, int height, Interpolation interpolation)
        :ImageProcess(processor, from, to), width(width), height(height), interpolation(interpolation)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "resize";}
    const ProcessInputType GetFromType() { return ProcessInputType::Memory; }
    const ProcessInputType GetToType() { return ProcessInputType::Memory; }
private:

    int width;
    int height;
    Interpolation interpolation;
};


#endif //NUVO_IMAGE_PROCESS_RESIZEIMAGEPROCESS_H
