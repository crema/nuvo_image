#include "ClearMemoryProcess.h"
#include "ImageProcessor.h"

const ImageProcessInput ClearMemoryProcess::Process(const ImageProcessInput &input, picojson::object &result) {
    processor->Clear();
    result["result"] = picojson::value(true);
    return input;
}