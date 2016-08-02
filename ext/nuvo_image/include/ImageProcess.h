#ifndef NUVO_IMAGE_IMAGEPROCESS_H
#define NUVO_IMAGE_IMAGEPROCESS_H

#include <unordered_map>
#include <string>
#include <memory>

#include "OpenCV.h"
#include "picojson.h"
#include "Enums.h"
#include "Gif.h"

class ImageProcessor;

class ImageProcessInput {
public:
    ImageProcessInput()
        :type(InvalidInput)
    {}

    ImageProcessInput(const std::shared_ptr<std::vector<unsigned char>> & buffer, const bool written)
        :buffer(buffer),type(File), written(written)
    {}

    ImageProcessInput(const std::shared_ptr<std::vector<unsigned char>> & buffer)
        :buffer(buffer),type(File), written(false)
    {}

    ImageProcessInput(const cv::Mat & mat)
        :mat(mat), type(Image)
    {}

    ImageProcessInput(const Gif & gif)
            :gif(gif), type(Animation)
    {}

    const std::shared_ptr<std::vector<unsigned char>> & GetBuffer() const { return buffer; }
    const cv::Mat & GetMat() const { return mat; }
    const Gif & GetGif() const { return gif; }

    const ProcessInputType GetType() const { return type; }

    bool Empty() const {
        ( buffer != nullptr && buffer->empty()) && mat.empty() && gif.Empty();
    }

    const int GetWidth() const {
        if(!gif.Empty())
            return gif.GetWidth();

        if(!mat.empty())
            return mat.cols;

        return -1;
    }

    const int GetHeight() const {
        if(!gif.Empty())
            return gif.GetHeight();

        if(!mat.empty())
            return mat.rows;

        return -1;
    }

    const bool IsWritten() const {
        return written;
    }
private:

    ProcessInputType type;
    std::shared_ptr<std::vector<unsigned char>> buffer;
    cv::Mat mat;
    Gif gif;
    bool written;
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
    virtual const int GetFromType() = 0;
    virtual const int GetToType() = 0;

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
