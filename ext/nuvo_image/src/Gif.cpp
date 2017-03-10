#include "Gif.h"
#include <cstring>

bool Gif::TryReadFromBuffer(const std::shared_ptr<std::vector<unsigned char>>& buffer, Gif& gif, Flatten flatten) {
  GifBuffer readBuffer(buffer->data(), buffer->size());
  int error = 0;
  auto gifFile = DGifOpen(&readBuffer, GifBuffer::ReadFromData, &error);

  if (gifFile == NULL)
    return false;

  if (DGifSlurp(gifFile) != GIF_OK)
    return false;

  cv::Mat mat(gifFile->SHeight, gifFile->SWidth, CV_8UC3);

  if (flatten == Flatten::Black) {
    mat.setTo(cv::Scalar(0, 0, 0));
  } else if (flatten == Flatten::White) {
    mat.setTo(cv::Scalar(255, 255, 255));
  }

  for (int i = 0; i < gifFile->ImageCount; ++i) {
    GraphicsControlBlock gcb;
    DGifSavedExtensionToGCB(gifFile, i, &gcb);
    mat = ReadFrame(mat.clone(), gifFile, i, gcb.TransparentColor);

    gif.frames->push_back(GifFrame(mat, gcb.DelayTime));
  }

  DGifCloseFile(gifFile, &error);
  return true;
}

cv::Mat Gif::ReadFrame(cv::Mat mat, GifFileType* gifFile, int index, int transparentColor) {
  auto gifImage = &gifFile->SavedImages[index];
  auto& imageDesc = gifImage->ImageDesc;

  const auto row = imageDesc.Top; /* Image Position relative to Screen. */
  const auto col = imageDesc.Left;
  const auto width = imageDesc.Width;
  const auto bottom = row + imageDesc.Height;

  auto colorMap = imageDesc.ColorMap == nullptr ? gifFile->SColorMap : imageDesc.ColorMap;

  unsigned char* srcPtr = gifImage->RasterBits;

  for (int i = row; i < bottom; ++i) {
    auto destPtr = mat.data + mat.step * i + mat.channels() * col;

    for (int x = 0; x < width; ++x) {
      SetPixel(destPtr, srcPtr, colorMap, transparentColor);
      destPtr += mat.channels();
      srcPtr++;
    }
  }

  return mat;
}

void Gif::SetPixel(unsigned char* dest, const unsigned char* src, ColorMapObject* colorMap, int transparentColor) {
  if (transparentColor >= 0 && transparentColor == *src)
    return;
  auto colorEntry = colorMap->Colors[*src];
  dest[0] = colorEntry.Blue;
  dest[1] = colorEntry.Green;
  dest[2] = colorEntry.Red;
}

int GifBuffer::ReadFromData(GifFileType* gif, GifByteType* bytes, int size) {
  return ((GifBuffer*)gif->UserData)->Read(bytes, size);
}

int GifBuffer::Read(GifByteType* bytes, int size) {
  if (length - position < size)
    size = length - position;

  std::memcpy(bytes, buffer + position, size);
  position += size;
  return size;
}
