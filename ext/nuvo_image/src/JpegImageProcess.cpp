#include "JpegImageProcess.h"
#include "ImageProcessor.h"

SIMMData::SIMMData(const cv::Mat &mat) {
    mat.convertTo(image, CV_32FC1);

    imageSquare = image.mul(image);
    GaussianBlur(image, imageBlur, cv::Size(11, 11), 1.5);
    imageBlurSquare = imageBlur.mul(imageBlur);

    GaussianBlur(imageSquare, sigma, cv::Size(11, 11), 1.5);
    sigma -= imageBlurSquare;
}

double SIMMData::GetSIMM(const SIMMData &a, const SIMMData &b) {
    const double C1 = 6.5025, C2 = 58.5225;

    auto & I1   = a.image;
    auto & I2   = b.image;

    auto & I1_2   = a.imageSquare;
    auto & I2_2   = b.imageSquare;
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    auto & mu1 = a.imageBlur;
    auto & mu2 = b.imageBlur;

    auto & mu1_2 = a.imageBlurSquare;
    auto & mu2_2 = b.imageBlurSquare;
    cv::Mat mu1_mu2 = mu1.mul(mu2);

    auto & sigma1_2 = a.sigma;
    auto & sigma2_2 = b.sigma;
    cv::Mat sigma12;

    GaussianBlur(I1_I2, sigma12, cv::Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;

    ///////////////////////////////// FORMULA ////////////////////////////////
    cv::Mat t1, t2, t3;

    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

    cv::Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;

    cv::Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return mssim[0];
}

int JpegQuality::GetQuality(const cv::Mat & image, std::vector<unsigned char> & buffer, const int minQuality, const int maxQuality, const int searchCount) {
    if(qualityType == QualityType::Fixed) {
        buffer.clear();
        JpegEncode(image, buffer, qualityFixed);
        return qualityFixed;
    }

    if(qualityType == QualityType::Adaptive) {
        switch(qualityAdaptive){
            case Quality::Low:
                qualitySIMM = 0.93;
                break;
            case Quality::Medium:
                qualitySIMM = 0.96;
                break;
            case Quality::High:
                qualitySIMM = 0.98;
                break;
            case Quality::VeryHigh:
                qualitySIMM = 0.999;
                break;
        }
    }

    cv::Mat imageGray;
    cv::cvtColor(image, imageGray, COLOR_RGB2GRAY);
    SIMMData imageSIMMData(imageGray);

    auto min = minQuality;
    auto max = maxQuality;
    auto current = max;

    for(int i; i< searchCount; ++i) {
        auto current = (min + max) / 2;
        auto currentSIMM = GetSimmByJpegQuality(imageSIMMData, image, buffer, current);

        if(max - min < 5 || std::abs(qualitySIMM - currentSIMM) < 0.005){
            return current;
        }

        if (qualitySIMM > currentSIMM){
            min = current;
        } else {
            max = current;
        }
    }
}

double JpegQuality::GetSimmByJpegQuality(const SIMMData & simmData, const cv::Mat & image, std::vector<unsigned char> & buffer, const int quality) {
    JpegEncode(image, buffer, quality);
    auto jpegGray = cv::imdecode(buffer, IMREAD_GRAYSCALE);
    SIMMData jpegSIMMData(jpegGray);
    return SIMMData::GetSIMM(simmData, jpegSIMMData);
}

void JpegQuality::JpegEncode(const cv::Mat &image, std::vector<unsigned char> &buffer, const int quality) {
    buffer.clear();
    std::vector<int> imageParams;
    imageParams.push_back(IMWRITE_JPEG_QUALITY); // IMWRITE_JPEG_QUALITY
    imageParams.push_back(quality);
    cv::imencode(".jpg",image, buffer, imageParams);
}


const ImageProcessInput JepegImageProcess::Process(const ImageProcessInput & input, picojson::object & result) {
    auto image = input.GetMat();

    std::vector<unsigned char> buffer;

    auto quality = jpegQuality.GetQuality(image, buffer, min, max, search);

    result["quality"] = picojson::value((double)quality);

    return ImageProcessInput(std::move(buffer));
}