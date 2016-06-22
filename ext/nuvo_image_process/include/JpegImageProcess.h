#ifndef NUVO_IMAGE_PROCESS_SAVEJPEGPROCESS_H
#define NUVO_IMAGE_PROCESS_SAVEJPEGPROCESS_H

#include "ImageProcess.h"
#include "Enums.h"

class JpegQuality{
public:
    enum QualityType {
        InvalidType = -1,
        Adaptive,
        Fixed,
    };

    JpegQuality()
        :qualityFixed(-1), qualityAdaptive(Quality::InvalidQuality), qualityType(QualityType::InvalidType)
    {}

    JpegQuality(Quality quality)
        :qualityFixed(-1), qualityAdaptive(quality), qualityType(Adaptive)
    {}

    JpegQuality(int quality)
        :qualityFixed(quality), qualityAdaptive(Quality::InvalidQuality), qualityType(QualityType::Fixed)
    {}

    int GetQuality(const cv::Mat & image, std::vector<unsigned char> & buffer);

private:
    double GetSIMM(const cv::Mat &source, const cv::Mat &dest);
    void JpegEncode(const cv::Mat &source,std::vector<unsigned char> & buffer, int quality);

    Quality  qualityAdaptive;
    int qualityFixed;
    QualityType qualityType;
};

class JepegImageProcess: public ImageProcess {
public:
    JepegImageProcess(std::shared_ptr<ImageProcessor> processor, const std::string & from, const std::string & to, const JpegQuality & jpegQuality)
        :ImageProcess(processor, from, to), jpegQuality(jpegQuality)
    {}

protected:
    const ImageProcessInput Process(const ImageProcessInput &input, picojson::object & result);
    const std::string GetName() { return "jpeg";}
    const ProcessInputType GetFromType() { return ProcessInputType::Memory; }
    const ProcessInputType GetToType() { return ProcessInputType::File; }

protected:
    JpegQuality jpegQuality;
};

#endif //NUVO_IMAGE_PROCESS_SAVEJPEGPROCESS_H
