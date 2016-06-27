#include <algorithm>
#include "Enums.h"

bool TryParse(const std::string & str, Gravity & out) {
    std::string converted = str;
    std::transform(converted.begin(), converted.end(), converted.begin(), tolower);

    if(converted == "center") {
        out = Gravity::Center;
    } else if (converted == "north") {
        out = Gravity::North;
    } else if (converted == "south") {
        out = Gravity::South;
    } else if (converted == "west"){
        out = Gravity::West;
    } else if (converted == "east"){
        out = Gravity::East;
    } else if (converted == "north_west" || converted =="northwest"){
        out = Gravity::NorthWest;
    } else if (converted == "north_east" || converted =="northeast"){
        out = Gravity::NorthEast;
    } else if (converted == "south_west" || converted =="southwest"){
        out = Gravity::SouthWest;
    } else if (converted == "south_east" || converted =="southeast"){
        out = Gravity::SouthEast;
    } else{
        out = Gravity::InvalidGravity;
    }
    return out != Gravity::InvalidGravity;
}

bool TryParse(const std::string & str, Quality & out) {
    std::string converted = str;
    std::transform(converted.begin(), converted.end(), converted.begin(), tolower);

    if(converted == "low") {
        out = Quality::Low;
    } else if (converted == "medium") {
        out = Quality::Medium;
    } else if (converted == "high") {
        out = Quality::High;
    } else if (converted == "very_high" || str == "very high"){
        out = Quality::VeryHigh;
    } else{
        out = Quality::InvalidQuality;
    }
    return out != Quality::InvalidQuality;
}

bool TryParse(const std::string & str, Interpolation & out) {
    std::string converted = str;
    std::transform(converted.begin(), converted.end(), converted.begin(), tolower);

    if( converted == "nearest") {
        out = Interpolation::Nearest;
    } else if(converted == "linear") {
        out = Interpolation::Linear;
    } else if (converted == "cubic") {
        out = Interpolation::Cubic;
    } else if (converted == "area") {
        out = Interpolation::Area;
    } else if (converted == "lanczos"){
        out = Interpolation::Lanczos;
    } else{
        out = Interpolation::InvalidInterpolation;
    }
    return out != Interpolation::InvalidInterpolation;
}

const std::string ToString(const Gravity gravity) {
    switch (gravity){
        case Gravity::Center:
            return "Center";
        case Gravity::North:
            return "North";
        case Gravity::South:
            return "South";
        case Gravity::West:
            return "West";
        case Gravity::East:
            return "East";
        case Gravity::NorthWest:
            return "NorthWest";
        case Gravity::NorthEast:
            return "NorthEast";
        case Gravity::SouthWest:
            return "SouthWest";
        case Gravity::SouthEast:
            return "SouthEast";
    }
    return "invalid";
}

const std::string ToString(const Quality quality) {
    switch (quality){
        case Quality::Low:
            return "low";
        case Quality::Medium:
            return "medium";
        case Quality::High:
            return "high";
        case Quality::VeryHigh:
            return "very_high";
    }
    return "invalid";
}
const std::string ToString(const Interpolation interpolation){
    switch (interpolation){
        case Interpolation::Nearest:
            return "nearest";
        case Interpolation::Linear:
            return "linear";
        case Interpolation::Cubic:
            return "cubic";
        case Interpolation::Area:
            return "area";
        case Interpolation::Lanczos:
            return "lanczos";
    }
    return "invalid";
}
