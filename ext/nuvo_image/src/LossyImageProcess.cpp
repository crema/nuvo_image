#include <cmath>
#include "LossyImageProcess.h"
#include "ImageProcessor.h"
#include "SSIM.h"


int LossyImageProcess::GetQuality(const std::shared_ptr<std::vector<unsigned char>> & buffer, const cv::Mat & image) {
    if(imageQuality.GetQualityType() == ImageQuality::QualityType::Fixed) {
        buffer->clear();
        Encode(buffer, image, imageQuality.GetQualityFixed());
        return imageQuality.GetQualityFixed();
    }

    auto qualitySSIM = imageQuality.GetQualitySSIM();

    if(imageQuality.GetQualityType() == ImageQuality::QualityType::Adaptive) {
        switch(imageQuality.GetQualityAdaptive()){
            case Quality::Low:
                qualitySSIM = 0.90;
                break;
            case Quality::Medium:
                qualitySSIM = 0.933;
                break;
            case Quality::High:
                qualitySSIM = 0.966;
                break;
            case Quality::VeryHigh:
                qualitySSIM = 0.999;
                break;
            default:
                throw std::runtime_error("invalid quality");
        }
    }

    SSIMData imageSSIMData = SSIMData(image);

    QualitySSIM min(minQuality, GetSimmByQuality(buffer, imageSSIMData, image, minQuality));
    if(min.ssim >= qualitySSIM) {
        return min.quality;
    }

    QualitySSIM max(maxQuality, GetSimmByQuality(buffer, imageSSIMData, image, maxQuality));
    if(max.ssim <= qualitySSIM) {
        return max.quality;
    }

    QualitySSIM current(maxQuality, 0);
    for(int i = 0; i< searchCount; ++i) {
        current.quality = QualitySSIM::InterpolationTargetSSIM(min, max, qualitySSIM);
        current.ssim = GetSimmByQuality(buffer, imageSSIMData, image, current.quality);

        if(max.quality - min.quality < 1 || std::abs(qualitySSIM - current.ssim) < 0.001) {
            return current.quality;
        }

        if (qualitySSIM > current.ssim){
            min = current;
        } else {
            max = current;
        }
    }
    return current.quality;
}

double LossyImageProcess::GetSimmByQuality(const std::shared_ptr<std::vector<unsigned char>> & buffer, const SSIMData &ssimData, const cv::Mat &image, const int quality) {
    Encode(buffer, image, quality);
    cv::Mat lossy = cv::imdecode(*buffer, cv::IMREAD_COLOR);
    SSIMData lossySSIMData(lossy);
    return SSIMData::GetSSIM(ssimData, lossySSIMData);
}

void LossyImageProcess::Encode(const std::shared_ptr<std::vector<unsigned char>> & buffer, const cv::Mat &image, const int quality) {
    buffer->clear();
    std::vector<int> imageParams;
    if(format == LossyImageFormat::Jpeg){
        imageParams.push_back(cv::IMWRITE_JPEG_QUALITY);
        imageParams.push_back(quality);
        cv::imencode(".jpg",image, *buffer, imageParams);
    } else if (format == LossyImageFormat::WebP){
        imageParams.push_back(cv::IMWRITE_WEBP_QUALITY);
        imageParams.push_back(quality);
        cv::imencode(".webp",image, *buffer, imageParams);
    } else {
        throw std::runtime_error("invalid image format");
    }
}

const ImageProcessInput LossyImageProcess::Process(const ImageProcessInput & input, picojson::object & result) {
    std::shared_ptr<std::vector<unsigned char>> buffer(new std::vector<unsigned char>());
    int quality = maxQuality;

    if(input.GetType() == ProcessInputType::Image){
        quality = GetQuality(buffer, input.GetMat());
    } else if(input.GetType() == ProcessInputType::Animation) {
        quality = GetQuality(buffer, input.GetGif().GetFrame(0).GetMat());
    } else {
        throw std::runtime_error("invalid lossy input");
    }

    result["quality"] = picojson::value((double)quality);
    return ImageProcessInput(buffer);
}