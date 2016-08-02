#ifndef NUVO_IMAGE_EXITPROCESS_H
#define NUVO_IMAGE_EXITPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"

class CloseProcess : public ImageProcess {
public:
    CloseProcess(std::shared_ptr<ImageProcessor> processor)
            :ImageProcess(processor, "", "")
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result){
        exit(0);
    }
    const std::string GetName() { return "close";}
    const int GetFromType() { return ProcessInputType::InvalidInput; }
    const int GetToType() { return ProcessInputType::InvalidInput; }
};


#endif //NUVO_IMAGE_EXITPROCESS_H
