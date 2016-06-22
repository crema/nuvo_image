#ifndef NUVO_IMAGE_PROCESS_EXITPROCESS_H
#define NUVO_IMAGE_PROCESS_EXITPROCESS_H

#include "ImageProcess.h"

class ExitProcess : public ImageProcess {
public:
    ExitProcess(std::shared_ptr<ImageProcessor> processor)
            :ImageProcess(processor, "", "")
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result){
        exit(0);
    }
    const std::string GetName() { return "exit";}
    const ProcessInputType GetFromType() { return ProcessInputType::None; }
    const ProcessInputType GetToType() { return ProcessInputType::None; }
};


#endif //NUVO_IMAGE_PROCESS_EXITPROCESS_H
