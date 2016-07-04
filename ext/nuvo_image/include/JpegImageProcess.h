#ifndef NUVO_IMAGE_SAVEJPEGPROCESS_H
#define NUVO_IMAGE_SAVEJPEGPROCESS_H

#include "opencv.h"
#include "ImageProcess.h"
#include "Enums.h"

struct QualitySSIM {
public:
    QualitySSIM(const int quality, const double ssim)
        :quality(quality), ssim(ssim)
    {}

    static int InterpolationTargetSSIM(const QualitySSIM & min, const QualitySSIM & max, const double targetSSIM);

    int quality;
    double ssim;
};

struct SSIMData {
public:
    SSIMData() {}
    SSIMData(const cv::Mat & mat);

    static double GetSSIM(const SSIMData & a, const SSIMData & b);
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
        Ssim,
        Fixed,
    };

    JpegQuality()
        :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(0), qualityType(QualityType::InvalidType)
    {}

    JpegQuality(Quality quality)
        :qualityFixed(0), qualityAdaptive(quality), qualitySSIM(0), qualityType(QualityType::Adaptive)
    {}

    JpegQuality(int quality)
        :qualityFixed(quality), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(0), qualityType(Fixed)
    {}

    JpegQuality(double quality)
            :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(quality), qualityType(QualityType::Ssim)
    {}

    int GetQuality(const cv::Mat & image, std::vector<unsigned char> & buffer, const int min, const int max, const int searchCount, const bool graySSIM);

private:

    double GetSimmByJpegQuality(const SSIMData & ssimData, const cv::Mat & image, std::vector<unsigned char> & buffer, const int quality, const bool graySSIM);
    void JpegEncode(const cv::Mat &source,std::vector<unsigned char> & buffer, const int quality);

    Quality  qualityAdaptive;
    int qualityFixed;
    double qualitySSIM;
    QualityType qualityType;
};

class JepegImageProcess: public ImageProcess {
public:
    JepegImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, const JpegQuality & jpegQuality, const int min, const int max, const int search, const bool graySSIM)
        :ImageProcess(processor, from, to), jpegQuality(jpegQuality), min(min), max(max), search(search), graySSIM(graySSIM)
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
    bool graySSIM;
};

#endif //NUVO_IMAGE_SAVEJPEGPROCESS_H
