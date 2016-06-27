#include <fstream>
#include "ReadImageProcess.h"
#include "ImageProcessor.h"
#include "exif.h"

const ImageProcessInput ReadImageProcess::Process(const ImageProcessInput &input, picojson::object &result) {
    auto buffer = input.GetBuffer();
    auto image = cv::imdecode(buffer, cv::IMREAD_COLOR);
    auto orientation = 0;

    result["width"] = picojson::value((double)image.cols);
    result["height"] = picojson::value((double)image.rows);

    if(autoOrient && TryReadExifOrientation(buffer, orientation)) {
        cv::Mat oriented;
        if(TryRotateOrientation(image, oriented, orientation)){
            image = oriented;
        }
        result["orientation"] = picojson::value((double)orientation);
    }
    return ImageProcessInput(image);
}



bool ReadImageProcess::TryReadExifOrientation(const std::vector<unsigned char> &buffer, int & orientation) {
    easyexif::EXIFInfo exif;
    int code = exif.parseFrom(buffer.data(),(int)buffer.size());
    if (code) {
        orientation = exif.Orientation;
        return true;
    }
    return false;
}

bool ReadImageProcess::TryRotateOrientation(const cv::Mat &src, cv::Mat & oriented, int orientation) {
    // http://www.impulseadventure.com/photo/exif-orientation.html
    cv::Mat transposed;

    switch(orientation){
        case 2:
            cv::flip(src, oriented, 1);
            break;
        case 3:
            cv::flip(src, oriented, -1);
            break;
        case 4:
            cv::flip(src, oriented, 0);
            break;
        case 5:
            cv::transpose(src, oriented);
            break;
        case 6:
            cv::transpose(src, transposed);
            cv::flip(transposed, oriented, 1);
            break;
        case 7:
            cv::transpose(src, transposed);
            cv::flip(transposed, oriented, -1);
            break;
        case 8:
            cv::transpose(src, transposed);
            cv::flip(transposed, oriented, 0);
            break;
    }
    if(oriented.empty()){
        return false;
    }

    return true;
}