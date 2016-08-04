#ifndef NUVO_IMAGE_SAVEJPEGPROCESS_H
#define NUVO_IMAGE_SAVEJPEGPROCESS_H

#include "OpenCV.h"
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

class ImageQuality {
public:
    enum QualityType {
        InvalidType = -1,
        Adaptive,
        Ssim,
        Fixed,
    };

    ImageQuality()
        :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(0), qualityType(QualityType::InvalidType)
    {}

    ImageQuality(Quality quality)
        :qualityFixed(0), qualityAdaptive(quality), qualitySSIM(0), qualityType(QualityType::Adaptive)
    {}

    ImageQuality(int quality)
        :qualityFixed(quality), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(0), qualityType(Fixed)
    {}

    ImageQuality(double quality, int min, int max, int search)
            :qualityFixed(0), qualityAdaptive(Quality::InvalidQuality), qualitySSIM(quality), qualityType(QualityType::Ssim)
    {}


    const Quality GetQualityAdaptive() const { return qualityAdaptive; }
    const int GetQualityFixed() const { return qualityFixed; }
    const double GetQualitySSIM() const { return qualitySSIM; }
    const QualityType GetQualityType() const { return qualityType; }

private:
    Quality  qualityAdaptive;
    int qualityFixed;
    double qualitySSIM;
    QualityType qualityType;
};

class LossyImageProcess: public ImageProcess {
public:
    LossyImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, const LossyImageFormat format, const ImageQuality & imageQuality, const int min, const int max, const int search)
        :ImageProcess(processor, from, to), format(format), imageQuality(imageQuality), minQuality(min), maxQuality(max), searchCount(search)
    {}

private:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "lossy";}
    const int GetFromType() { return ProcessInputType::Image | ProcessInputType::Animation; }
    const int GetToType() { return ProcessInputType::File; }

    int GetQuality(const std::shared_ptr<std::vector<unsigned char>> & buffer, const cv::Mat & image);
    double GetSimmByQuality(const std::shared_ptr<std::vector<unsigned char>> & buffer, const SSIMData & ssimData, const cv::Mat & image, const int quality);
    void Encode(const std::shared_ptr<std::vector<unsigned char>> & buffer, const cv::Mat &source, const int quality);


    LossyImageFormat format;
    ImageQuality imageQuality;
    int minQuality;
    int maxQuality;
    int searchCount;
};

#endif //NUVO_IMAGE_SAVEJPEGPROCESS_H
