#ifndef NUVO_IMAGE_SSIM_H
#define NUVO_IMAGE_SSIM_H

#include "OpenCV.h"
#include "Enums.h"

struct QualitySSIM {
 public:
  QualitySSIM(const int quality, const double ssim) : quality(quality), ssim(ssim) {}

  static int InterpolationTargetSSIM(const QualitySSIM& min, const QualitySSIM& max, const double targetSSIM);

  int quality;
  double ssim;
};

struct SSIMData {
 public:
  SSIMData() {}
  SSIMData(const cv::Mat& mat);

  static double GetSSIM(const SSIMData& a, const SSIMData& b);

 private:
  cv::Mat image;
  cv::Mat imageSquare;
  cv::Mat imageBlur;
  cv::Mat imageBlurSquare;
  cv::Mat sigma;
};
#endif  // NUVO_IMAGE_SSIM_H
