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
    None = -1,
    File,
    Memory
};


extern bool TryParse(const std::string & str, Gravity & out);
extern bool TryParse(const std::string & str, Quality & out);
extern bool TryParse(const std::string & str, Interpolation & out);
extern const std::string ToString(const Gravity gravity);
extern const std::string ToString(const Quality quality);
extern const std::string ToString(const Interpolation interpolation);

#endif //NUVO_IMAGE_ENUMS_H
