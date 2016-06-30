#ifndef NUVO_IMAGE_IMAGEPROCESS_H
#define NUVO_IMAGE_IMAGEPROCESS_H

#include <unordered_map>
#include <string>
#include <memory>

#include "opencv.h"
#include "picojson.h"
#include "Enums.h"

class ImageProcessor;

class ImageProcessInput {
public:
    ImageProcessInput()
        :type(InvalidInput)
    {}

    ImageProcessInput(const std::vector<unsigned char> & buffer)
        :buffer(buffer),type(File)
    {}

    ImageProcessInput(const cv::Mat & mat)
        :mat(mat), type(Memory)
    {}

    const std::vector<unsigned char> & GetBuffer() const { return buffer; }
    const cv::Mat & GetMat() const { return mat; }

    ProcessInputType GetType() { return type; }
private:
    ProcessInputType type;
    std::vector<unsigned char> buffer;
    cv::Mat mat;
};


class ImageProcess {
public:
    ImageProcess(std::shared_ptr<ImageProcessor> processor,const std::string & from, const std::string & to)
        :processor(processor), from(from), to(to)
    {}

    picojson::object Process();

protected:
    virtual const ImageProcessInput Process(const ImageProcessInput & input, picojson::object & result) = 0;
    virtual const std::string GetName() = 0;
    virtual const ProcessInputType GetFromType() = 0;
    virtual const ProcessInputType GetToType() = 0;

    bool RequireFrom() { return GetFromType() != ProcessInputType::InvalidInput;}
    bool RequireTo() { return GetToType() != ProcessInputType::InvalidInput; }
    bool HasFrom() { return !from.empty();}
    bool HasTo() { return !to.empty(); }

    const ImageProcessInput ReadFrom();
    void WriteTo(const ImageProcessInput & input);

    std::shared_ptr<ImageProcessor> processor;
    std::string from;
    std::string to;
};

#endif
