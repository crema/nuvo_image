#ifndef NUVO_IMAGE_IMGAEPROCESSOR_H
#define NUVO_IMAGE_IMGAEPROCESSOR_H

#include <unordered_map>
#include <string>
#include <memory>

#include "opencv.h"
#include "picojson.h"
#include "Enums.h"
#include "JpegImageProcess.h"
#include "CropImageProcess.h"


class ImageProcess;

class ImageProcessor: public std::enable_shared_from_this<ImageProcessor> {
public:
    ImageProcessor()
        :tempCount(0)
    {}

    std::shared_ptr<ImageProcess> Parse(const std::string & inputString);

    bool TryGet(const std::string & name,cv::Mat & outMat);
    void Insert(const std::string & name, const cv::Mat & mat);
    void Clear();
    const std::string GetTempName() { return "temp-" + std::to_string(tempCount++); }

private:
    int tempCount;


    template<typename T>
    inline bool TryGet(const picojson::value & value, T & out) {
        if(!value.is<T>())
            return false;
        out = value.get<T>();
        return true;
    }


    template<typename T>
    inline T GetMember(picojson::object & object, const std::string & name) {
        T out;
        if(!TryGetMember(object, name, out)) {
            throw std::runtime_error("invalid: " + name);
        }
        return out;
    }

    template<typename T>
    inline T GetMember(picojson::object & object, const std::string & name, T defaultValue) {
        T out;
        if(!TryGetMember(object, name, out)) {
            return defaultValue;
        }
        return out;
    }

    template<typename T>
    inline bool TryGetMember(picojson::object & object, const std::string & name, T & out) {
        auto value = object[name];
        return TryGet(value, out);
    }


    std::unordered_map<std::string, cv::Mat> data;
};

template <>
inline bool ImageProcessor::TryGet<Flatten>(const picojson::value & value, Flatten & out) {
    if(!value.is<std::string>()) {
        return false;
    }
    return TryParse(value.get<std::string>(), out);
}

template <>
inline bool ImageProcessor::TryGet<Gravity>(const picojson::value & value, Gravity & out) {
    if(!value.is<std::string>()) {
        return false;
    }
    return TryParse(value.get<std::string>(), out);
}


template <>
inline bool ImageProcessor::TryGet<Interpolation>(const picojson::value & value, Interpolation & out) {
    if(!value.is<std::string>()) {
        return false;
    }
    return TryParse(value.get<std::string>(), out);
}


template <>
inline bool ImageProcessor::TryGet<JpegQuality>(const picojson::value & value, JpegQuality & out) {
    if(value.is<std::string>()) {
        Quality quality;
        if(TryParse(value.get<std::string>(), quality)) {
            out = JpegQuality(quality);
            return true;
        }
    } else if(value.is<double>()) {
        auto quality = value.get<double>();

        if(quality >=0 && quality <= 1) {
            out = JpegQuality(quality);
            return true;
        } else if(quality > 1 && quality <= 100){
            out = JpegQuality((int)quality);
            return true;
        }
    }
    return false;
}


#endif