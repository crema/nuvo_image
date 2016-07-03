#ifndef NUVO_IMAGE_SAVEJPEGPROCESS_H
#define NUVO_IMAGE_SAVEJPEGPROCESS_H

#include "opencv.h"
#include "ImageProcess.h"
#include "Enums.h"

class SIMMData {
public:
    SIMMData(const cv::Mat & mat);

    static double GetSIMM(const SIMMData & a, const SIMMData & b);
private:
    cv::Mat image;
    cv::Mat imageSquare;
    cv::Mat imageBlur;
    cv::Mat imageBlurSquare;
    cv::Mat sigma;
};

class JpegQuality {
public:
    enum QualityType {
        InvalidType = -1,
        Adaptive,
        Simm,
        Fixed,
    };

    JpegQuality()
        :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySIMM(0), qualityType(QualityType::InvalidType)
    {}

    JpegQuality(Quality quality)
        :qualityFixed(0), qualityAdaptive(quality), qualitySIMM(0), qualityType(QualityType::Adaptive)
    {}

    JpegQuality(int quality)
        :qualityFixed(quality), qualityAdaptive(Quality::InvalidQuality), qualitySIMM(0), qualityType(Fixed)
    {}

    JpegQuality(double quality)
            :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySIMM(quality), qualityType(QualityType::Simm)
    {}

    int GetQuality(const cv::Mat & image, std::vector<unsigned char> & buffer, const int min, const int max, const int searchCount);

private:

    double GetSimmByJpegQuality(const SIMMData & simmData, const cv::Mat & image, std::vector<unsigned char> & buffer, const int quality);
    void JpegEncode(const cv::Mat &source,std::vector<unsigned char> & buffer, const int quality);

    Quality  qualityAdaptive;
    int qualityFixed;
    double qualitySIMM;
    QualityType qualityType;
};

class JepegImageProcess: public ImageProcess {
public:
    JepegImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, const JpegQuality & jpegQuality, const int min, const int max, const int search)
        :ImageProcess(processor, from, to), jpegQuality(jpegQuality), min(min), max(max), search(search)
    {}

private:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "jpeg";}
    const ProcessInputType GetFromType() { return ProcessInputType::Memory; }
    const ProcessInputType GetToType() { return ProcessInputType::File; }

    JpegQuality jpegQuality;
    int min;
    int max;
    int search;
};

#endif //NUVO_IMAGE_SAVEJPEGPROCESS_H
