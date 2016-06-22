#include <cv.hpp>
#include "ImageProcessor.h"
#include "ReadImageProcess.h"
#include "CropImageProcess.h"
#include "ResizeImageProcess.h"
#include "JpegImageProcess.h"
#include "ClearMemoryProcess.h"
#include "CloseProcess.h"

std::shared_ptr<ImageProcess> ImageProcessor::Parse(const std::string & inputString) {
    picojson::value json;
    auto error = picojson::parse(json, inputString);

    if(!error.empty())
        throw std::runtime_error(error);

    picojson::object object;
    if(!TryGet(json, object)) {
        throw std::runtime_error("invalid json");
    }

    auto processString = GetMember<std::string>(object, "process");
    auto from = GetMember<std::string>(object, "from", "");
    auto to = GetMember<std::string>(object, "to", "");

    if(processString == "read") {
        auto autoOrient = GetMember<bool>(object,"auto_orient", false);

        return std::shared_ptr<ImageProcess>(new ReadImageProcess(shared_from_this(), from, to,  autoOrient));
    } else if (processString == "crop") {
        auto width = (int)GetMember<double>(object, "width");
        auto height = (int)GetMember<double>(object, "height");
        auto gravity = GetMember<Gravity>(object, "gravity", Gravity::Center);

        return std::shared_ptr<ImageProcess>(new CropImageProcess(shared_from_this(), from, to, width, height, gravity));
    } else if (processString == "resize") {
        auto width = (int)GetMember<double>(object, "width");
        auto height = (int)GetMember<double>(object, "height");
        auto interpolation = GetMember<Interpolation>(object, "interpolation", Interpolation::Area);

        return std::shared_ptr<ImageProcess>(new ResizeImageProcess(shared_from_this(), from, to, width, height, interpolation));
    } else if (processString == "jpeg") {
        auto skipIfBigger = GetMember<bool>(object, "skip_if_bigger", false);
        auto jpegQuality = GetMember<JpegQuality>(object, "quality", JpegQuality(95));

        return std::shared_ptr<ImageProcess>(new JepegImageProcess(shared_from_this(), from, to, jpegQuality));
    } else if (processString == "clear") {
        return std::shared_ptr<ImageProcess>(new ClearMemoryProcess(shared_from_this()));
    }
    else if (processString == "close") {
        return std::shared_ptr<ImageProcess>(new CloseProcess(shared_from_this()));
    }

    throw std::runtime_error("invalid input");
}

bool ImageProcessor::TryGet(const std::string &name, cv::Mat & outMat) {
    auto findData = data.find(name);

    if(findData == data.end())
        return false;

    outMat = findData->second;
    return true;
}

void ImageProcessor::Insert(const std::string &name, const cv::Mat &mat) {
    data.insert(std::make_pair(name, mat));
}

void ImageProcessor::Clear() {
    data.clear();
}
