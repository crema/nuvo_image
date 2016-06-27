#include "JpegImageProcess.h"
#include "ImageProcessor.h"


int JpegQuality::GetQuality(const cv::Mat & image, std::vector<unsigned char> & buffer) {
    if(qualityType == QualityType::Fixed){
        buffer.clear();
        JpegEncode(image, buffer, qualityFixed);
        return qualityFixed;
    }

    auto targetSSIM = 1.0;

    switch(qualityAdaptive){
        case Quality::Low:
            targetSSIM = 0.98;
            break;
        case Quality::Medium:
            targetSSIM = 0.99;
            break;
        case Quality::High:
            targetSSIM = 0.995;
            break;
        case Quality::VeryHigh:
            targetSSIM = 0.999;
            break;
    }

    auto minQuality = 50;
    auto maxQuality = 100;

    auto lastQuality = 0;
    auto lastSSIM = 0;

    cv::Mat imageGray;
    cv::cvtColor(image, imageGray, COLOR_RGB2GRAY);

    auto currentQuality = 100;
    while(true) {
        currentQuality = (minQuality + maxQuality) / 2;
        buffer.clear();
        JpegEncode(image, buffer, currentQuality);

        if(abs(maxQuality - minQuality) <= 2){
            return currentQuality;
        }

        auto jpegGray = cv::imdecode(buffer, IMREAD_GRAYSCALE);
        auto currentSSIM = GetSIMM(imageGray, jpegGray);

        if(currentSSIM < targetSSIM) {
            minQuality = currentQuality;
        } else {
            maxQuality = currentQuality;
        }
    }
    return currentQuality;
}

void JpegQuality::JpegEncode(const cv::Mat &image, std::vector<unsigned char> &buffer, int quality) {
    std::vector<int> imageParams;
    imageParams.push_back(IMWRITE_JPEG_QUALITY); // IMWRITE_JPEG_QUALITY
    imageParams.push_back(quality);
    cv::imencode(".jpg",image, buffer, imageParams);
}


double JpegQuality::GetSIMM(const cv::Mat &source, const cv::Mat &dest) {
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;

    cv::Mat I1, I2;
    source.convertTo(I1, d);            // cannot calculate on one byte large values
    dest.convertTo(I2, d);

    cv::Mat I2_2   = I2.mul(I2);        // I2^2
    cv::Mat I1_2   = I1.mul(I1);        // I1^2
    cv::Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/

    cv::Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);

    cv::Mat mu1_2   =   mu1.mul(mu1);
    cv::Mat mu2_2   =   mu2.mul(mu2);
    cv::Mat mu1_mu2 =   mu1.mul(mu2);

    cv::Mat sigma1_2, sigma2_2, sigma12;

    GaussianBlur(I1_2, sigma1_2, cv::Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;

    GaussianBlur(I2_2, sigma2_2, cv::Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;

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


const ImageProcessInput JepegImageProcess::Process(const ImageProcessInput & input, picojson::object & result) {
    auto image = input.GetMat();

    std::vector<unsigned char> buffer;

    auto quality = jpegQuality.GetQuality(image, buffer);

    result["quality"] = picojson::value((double)quality);

    return ImageProcessInput(std::move(buffer));
}