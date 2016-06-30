#ifndef NUVO_IMAGE_CLEARMEMORYPROCESS_H
#define NUVO_IMAGE_CLEARMEMORYPROCESS_H

#include "opencv.h"
#include "ImageProcess.h"

class ClearMemoryProcess : public ImageProcess {
public:
    ClearMemoryProcess(std::shared_ptr<ImageProcessor> processor)
        :ImageProcess(processor, "", "")
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "clear";}
    const ProcessInputType GetFromType() { return ProcessInputType::InvalidInput; }
    const ProcessInputType GetToType() { return ProcessInputType::InvalidInput; }
};


#endif //NUVO_IMAGE_CLEARMEMORYPROCESS_H
