#include "SSIM.h"

int QualitySSIM::InterpolationTargetSSIM(const QualitySSIM &min, const QualitySSIM &max, const double targetSSIM) {
    auto slope = (max.quality - min.quality) / (max.ssim - min.ssim);
    return (int)round(slope * (targetSSIM - min.ssim)) + min.quality;
}

SSIMData::SSIMData(const cv::Mat &mat) {
    mat.convertTo(image, CV_32F);

    imageSquare = image.mul(image);
    GaussianBlur(image, imageBlur, cv::Size(11, 11), 1.5);
    imageBlurSquare = imageBlur.mul(imageBlur);

    GaussianBlur(imageSquare, sigma, cv::Size(11, 11), 1.5);
    sigma -= imageBlurSquare;
}

double SSIMData::GetSSIM(const SSIMData &a, const SSIMData &b) {
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

    if(mssim[1] == 0 && mssim[2] == 0 && mssim[3] == 0){
        return mssim[0];
    } else {
        auto ssim = (mssim[0] + mssim[1] + mssim[2]) / 3;
        return ssim;
    }
}