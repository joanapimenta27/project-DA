#include "Station.h"

Station::Station(const std::string &id, const std::string &code){
    id_ = id;
    code_ = code;
}

std::string Station::getId(){
    return id_;
}

std::string Station::getCode() {
    return code_;
}

Station::Station() = default;
