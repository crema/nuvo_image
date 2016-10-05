#ifndef NUVO_IMAGE_CLEARMEMORYPROCESS_H
#define NUVO_IMAGE_CLEARMEMORYPROCESS_H

#include "OpenCV.h"
#include "ImageProcess.h"

class ClearMemoryProcess : public NullToNullProcess {
public:
    ClearMemoryProcess(std::shared_ptr<ImageProcessor> processor)
        :NullToNullProcess(processor)
    {}

protected:
    void Process(picojson::object & result){
        processor->Clear();
        result["result"] = picojson::value(true);
    }
    const std::string GetName() { return "clear";}
};


#endif //NUVO_IMAGE_CLEARMEMORYPROCESS_H
