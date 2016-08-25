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
        return ( buffer == nullptr || buffer->empty()) && mat.empty() && gif.Empty();
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
    ImageProcess(const std::shared_ptr<ImageProcessor> & processor)
        :processor(processor)
    {}

    virtual picojson::object Process() = 0;
protected:
    virtual const std::string GetName() = 0;

    const ImageProcessInput ReadFrom(const std::string & fromString, const int fromType);
    void WriteTo(const std::string & toString, const int toType, const ImageProcessInput & output);


    std::shared_ptr<ImageProcessor> processor;
};

class InOutProcess: public ImageProcess{
public:
    InOutProcess(const std::shared_ptr<ImageProcessor> & processor, const std::string & from, const std::string & to)
        :ImageProcess(processor), from(from), to(to)
    {}

    picojson::object Process();

protected:
    virtual const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result) = 0;

    virtual const int GetFromType() = 0;
    virtual const int GetToType() = 0;

    std::string from;
    std::string to;
};


class TwoImageToNullProcess: public ImageProcess {
public:
    TwoImageToNullProcess(const std::shared_ptr<ImageProcessor> & processor, const std::string & from1, const std::string & from2)
            :ImageProcess(processor), from1(from1), from2(from2)
    {}

    picojson::object Process();
protected:
    virtual void Process(const ImageProcessInput & input1, const ImageProcessInput & input2, picojson::object & result) = 0;

    std::string from1;
    std::string from2;
};

class NullToNullProcess: public ImageProcess {
public:
    NullToNullProcess(const std::shared_ptr<ImageProcessor> & processor)
            :ImageProcess(processor)
    {}

    picojson::object Process();
protected:
    virtual void Process(picojson::object & result) = 0;

};

#endif
