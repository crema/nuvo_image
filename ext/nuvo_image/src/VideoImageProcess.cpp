#include <fstream>
#include <iostream>
#include <cstdio>
#include <string>
#include "VideoImageProcess.h"


const ImageProcessInput VideoImageProcess::Process(const ImageProcessInput &input, picojson::object &result) {

    Gif gif = input.GetGif();
    cv::VideoWriter writer;

    int minDelay = 9999;

    for(int i = 0; i < gif.GetFrameCount(); ++i) {
        minDelay = std::min(gif.GetFrame(i).GetDelay(), minDelay);
    }

    auto fps = 100.0 / minDelay;


    if(format == VideoFormat::Mp4) {
        if(!writer.open(to,
                    cv::VideoWriter::fourcc('X','2','6','4'),
                    fps,
                    cv::Size(gif.GetWidth(), gif.GetHeight()))) {
            throw std::runtime_error("cant open x264");
        }

    } else if (format == VideoFormat::WebM){
        if(!writer.open(to,
                    cv::VideoWriter::fourcc('V','P','8','0'),
                    fps,
                    cv::Size(gif.GetWidth(), gif.GetHeight()))) {
            throw std::runtime_error("cant open vp8");
        }
    } else {
        throw std::runtime_error("invalid video format");
    }

    auto delayPerFrame = 1000/fps;

    for(int i = 0; i < gif.GetFrameCount(); ++i) {
        auto frame = gif.GetFrame(i);
        auto frameCount = (int)round(frame.GetDelay() * 10/ delayPerFrame);

        for(int j=0; j < frameCount; ++j) {
            writer.write(frame.GetMat());
        }
    }

    writer.release();

    result["format"] = picojson::value(ToString(format));

    std::ifstream file(to, std::ios::in | std::ios::binary);

    if(!file.good()) {
        throw std::runtime_error("can not open file: " + to);
    }
    file.seekg(0, std::ios::end);
    auto size = (int)file.tellg();
    file.seekg(0, std::ios::beg);

    std::shared_ptr<std::vector<unsigned char>> buffer(new std::vector<unsigned char>(size));
    file.close();
    return ImageProcessInput(buffer, true);
}