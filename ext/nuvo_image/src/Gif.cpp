#include "Gif.h"


bool Gif::TryReadFromBuffer(const std::shared_ptr<std::vector<unsigned char>> & buffer, Gif & gif,Flatten flatten) {
    GifBuffer readBuffer(buffer->data(), buffer->size());
    int error = 0;
    auto gifFile = DGifOpen(&readBuffer,GifBuffer::ReadFromData, &error);

    if(gifFile == NULL)
        return false;

    if(DGifSlurp(gifFile) != GIF_OK)
        return false;

    for(int i = 0; i< gifFile->ImageCount; ++i){
        GraphicsControlBlock gcb;
        DGifSavedExtensionToGCB(gifFile,i, &gcb);
        auto mat = ReadFrame(gifFile, i, gcb.TransparentColor, flatten);

        gif.frames->push_back(GifFrame(mat, gcb.DelayTime));
    }

    DGifCloseFile(gifFile, &error);
    return true;
}

cv::Mat Gif::ReadFrame(GifFileType *gif, int index, int transparentColor, Flatten flatten) {
    cv::Mat mat(gif->SHeight,gif->SWidth, CV_8UC3);

    if(flatten == Flatten::Black){
        mat.setTo(cv::Scalar(0,0,0));
    }else if(flatten == Flatten::White){
        mat.setTo(cv::Scalar(255,255,255));
    }

    auto gifImage = &gif->SavedImages[index];

    auto row = gif->Image.Top; /* Image Position relative to Screen. */
    auto col = gif->Image.Left;
    auto width = gif->Image.Width;
    auto height = gif->Image.Height;

    auto colorMap = gifImage->ImageDesc.ColorMap == nullptr ? gif->SColorMap : gifImage->ImageDesc.ColorMap;

    unsigned  char * srcPtr = gifImage->RasterBits;
    unsigned  char * srcEndPtr = gifImage->RasterBits + gifImage->ImageDesc.Width * gifImage->ImageDesc.Height;

    if(gifImage->ImageDesc.Interlace){
        int interlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
        int interlacedJumps[] = { 8, 8, 4, 2 }; /* be read - offsets and jumps... */

        while( srcPtr < srcEndPtr){
            for (int count,i = 0; i < 4; i++){
                for (int j = row + interlacedOffset[i]; j < row + height;
                     j += interlacedJumps[i]) {

                    auto destPtr = mat.data + mat.step * j + mat.channels() * col;
                    for (int x = 0; x< width; ++x){
                        SetPixel(destPtr, srcPtr, colorMap, transparentColor);
                        destPtr += mat.channels();
                        srcPtr++;
                    }
                }
            }
        }
    } else {
        for (int i = 0; i < height; i++) {
            auto destPtr = mat.data + mat.step * row++ + mat.channels() * col;
            for (int x = 0; x< width; ++x){
                SetPixel(destPtr, srcPtr, colorMap, transparentColor);
                destPtr += mat.channels();
                srcPtr++;
            }
        }
    }

    return mat;
}

void Gif::SetPixel(unsigned char * dest, const unsigned char *src, ColorMapObject * colorMap, int transparentColor){
    if(transparentColor >= 0 && transparentColor == *src)
        return;
    auto colorEntry = colorMap->Colors[*src];
    dest[0] = colorEntry.Blue;
    dest[1] = colorEntry.Green;
    dest[2] = colorEntry.Red;
}

int Gif::GifBuffer::ReadFromData(GifFileType* gif, GifByteType* bytes, int size) {
    return ((GifBuffer*)gif->UserData)->Read(bytes,size);
}

int Gif::GifBuffer::Read(GifByteType* bytes, int size){
    if (length - position < size)
        size = length - position;

    memcpy(bytes, buffer + position, size);
    position += size;
    return size;
}

