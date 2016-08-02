//
// Created by ubuntu on 16. 7. 28.
//

#ifndef NUVO_IMAGE_FRAMEPROCESS_H
#define NUVO_IMAGE_FRAMEPROCESS_H


#include "OpenCV.h"
#include "ImageProcess.h"

class FrameImageProcess : public ImageProcess {
public:
    FrameImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string to, int frame)
            :ImageProcess(processor, from, to), frame(frame)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "frame";}
    const int GetFromType() { return ProcessInputType::Animation; }
    const int GetToType() { return ProcessInputType::Image; }

    int frame;
};


#endif //NUVO_IMAGE_GETFRAMEPROCESS_H
