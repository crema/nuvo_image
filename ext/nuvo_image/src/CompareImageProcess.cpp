#include "CompareImageProcess.h"
#include "SSIM.h"

void CompareImageProcess::Process(const ImageProcessInput & input1, const ImageProcessInput & input2, picojson::object & result) {
    auto image1 = input1.GetType() == ProcessInputType::Animation ? input1.GetGif().GetFrame(0).GetMat() : input1.GetMat();
    auto image2 = input2.GetType() == ProcessInputType::Animation ? input2.GetGif().GetFrame(0).GetMat() : input2.GetMat();

    SSIMData image1SSIMData(image1);
    SSIMData image2SSIMData(image2);

    result["ssim"] = picojson::value(SSIMData::GetSSIM(image1SSIMData, image2SSIMData));
}
