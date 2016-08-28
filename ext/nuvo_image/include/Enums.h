#ifndef NUVO_IMAGE_ENUMS_H
#define NUVO_IMAGE_ENUMS_H

#include <string>

enum Gravity{
    InvalidGravity = -1,
    Center =            0x0000,

    North =             0x1000,
    South =             0x0100,

    West =              0x0010,
    East =              0x0001,

    NorthWest =         North | West,
    NorthEast =         North | East,

    SouthWest =         South | West,
    SouthEast =         South | East,
};

enum Quality{
    InvalidQuality = -1,
    Low,
    Medium,
    High,
    VeryHigh,
};

enum Interpolation{
    InvalidInterpolation = -1,
    Nearest = 0,
    Linear = 1,
    Cubic = 2,
    Area = 3,
    Lanczos = 4,
};

enum ProcessInputType{
    InvalidInput = -1,
    File = 0x001,
    Image = 0x010,
    Animation = 0x100,
};

enum Flatten {
    None = -1,
    White,
    Black
};


enum LossyImageFormat {
    InvalidLossyImageFormat = -1,
    Jpeg,
    LossyWebP
};

enum LosslessImageFormat {
    InvalidLosslessImageFormat = -1,
    Png,
    LosslessWebP
};

enum VideoFormat {
    InvalidVideoFormat = -1,
    Mp4,
    WebM
};

extern bool TryParse(const std::string & str, Flatten & out);
extern bool TryParse(const std::string & str, Gravity & out);
extern bool TryParse(const std::string & str, Quality & out);
extern bool TryParse(const std::string & str, Interpolation & out);
extern bool TryParse(const std::string & str, LossyImageFormat & out);
extern bool TryParse(const std::string & str, LosslessImageFormat & out);
extern bool TryParse(const std::string & str, VideoFormat & out);

extern const std::string ToString(const Flatten flatten);
extern const std::string ToString(const Gravity gravity);
extern const std::string ToString(const Quality quality);
extern const std::string ToString(const Interpolation interpolation);
extern const std::string ToString(const LossyImageFormat format);
extern const std::string ToString(const LosslessImageFormat format);
extern const std::string ToString(const VideoFormat format);

#endif //NUVO_IMAGE_ENUMS_H
