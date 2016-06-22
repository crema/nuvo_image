#include "Enums.h"

bool TryParse(const std::string & str, Gravity & out) {
    if(str == "center") {
        out = Gravity::Center;
    } else if (str == "north") {
        out = Gravity::North;
    } else if (str == "south") {
        out = Gravity::South;
    } else if (str == "west"){
        out = Gravity::West;
    } else if (str == "east"){
        out = Gravity::East;
    } else if (str == "north_west" || str =="north west"){
        out = Gravity::NorthWest;
    } else if (str == "north_east" || str =="north east"){
        out = Gravity::NorthEast;
    } else if (str == "south_west" || str =="south west"){
        out = Gravity::SouthWest;
    } else if (str == "south_east" || str =="south east"){
        out = Gravity::SouthEast;
    } else{
        out = Gravity::InvalidGravity;
    }
    return out != Gravity::InvalidGravity;
}

bool TryParse(const std::string & str, Quality & out) {
    if(str == "low") {
        out = Quality::Low;
    } else if (str == "medium") {
        out = Quality::Medium;
    } else if (str == "high") {
        out = Quality::High;
    } else if (str == "very_high" || str == "very high"){
        out = Quality::VeryHigh;
    } else{
        out = Quality::InvalidQuality;
    }
    return out != Quality::InvalidQuality;
}

bool TryParse(const std::string & str, Interpolation & out) {
    if( str == "nearest") {
        out = Interpolation::Nearest;
    } else if(str == "linear") {
        out = Interpolation::Linear;
    } else if (str == "cubic") {
        out = Interpolation::Cubic;
    } else if (str == "area") {
        out = Interpolation::Area;
    } else if (str == "lanczos"){
        out = Interpolation::Lanczos;
    } else{
        out = Interpolation::InvalidInterpolation;
    }
    return out != Interpolation::InvalidInterpolation;
}

const std::string ToString(const Gravity gravity) {
    switch (gravity){
        case Gravity::Center:
            return "center";
        case Gravity::North:
            return "north";
        case Gravity::South:
            return "south";
        case Gravity::West:
            return "west";
        case Gravity::East:
            return "east";
        case Gravity::NorthWest:
            return "north_west";
        case Gravity::NorthEast:
            return "north_east";
        case Gravity::SouthWest:
            return "south_west";
        case Gravity::SouthEast:
            return "south_east";
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
            return "Lanczos";
    }
    return "invalid";
}
