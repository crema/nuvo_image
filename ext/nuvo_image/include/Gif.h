//
// Created by ubuntu on 16. 7. 28.
//

#ifndef NUVO_IMAGE_GIF_H
#define NUVO_IMAGE_GIF_H

#include "gif_lib.h"
#include <vector>
#include <memory>
#include "OpenCV.h"
#include "Enums.h"


class Gif {
public:
    class GifFrame{
    public:
        GifFrame(const cv::Mat & mat, const int delay)
                :mat(mat), delay(delay)
        {}

        const cv::Mat & GetMat() const {
            return mat;
        }

        const int GetDelay() const {
            return delay;
        }

    private:
        cv::Mat mat;
        int delay;
    };

    static bool TryReadFromBuffer(const std::shared_ptr<std::vector<unsigned char>> & buffer, Gif & gif,Flatten flatten=Flatten::White);

    Gif()
        :frames(new std::vector<GifFrame>())
    {}

    const GifFrame & GetFrame(int i) const {
        return (*frames)[i];
    };

    const int GetFrameCount() const {
        return frames->size();
    }

    bool Empty() const {
        return frames->empty();
    }

    void AddFrame(const GifFrame &frame) {
        frames->push_back(frame);
    }

    const int GetWidth() const {
        if(Empty())
            return -1;
        return GetFrame(0).GetMat().cols;
    }

    const int GetHeight() const {
        if(Empty())
            return -1;
        return GetFrame(0).GetMat().rows;
    }
private:

    class GifBuffer {
    public:
        GifBuffer(unsigned char * buffer, int length)
                :buffer(buffer), length(length), position(0)
        {}

        static int ReadFromData(GifFileType* gif, GifByteType* bytes, int size);
        int Read(GifByteType* bytes, int size);
    private:

        unsigned char * buffer;
        int position;
        int length;
    };

    static cv::Mat ReadFrame(GifFileType *gif, int index, int transparentColor, Flatten flatten);
    static void SetPixel(unsigned char * dest, const unsigned char *src, ColorMapObject * colorMap, int transparentColor);

    std::shared_ptr<std::vector<GifFrame>> frames;
};

#endif //NUVO_IMAGE_GIF_H
