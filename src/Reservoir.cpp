#include "Reservoir.h"

Reservoir::Reservoir(const std::string &name, const std::string &municipality, const std::string &id, const std::string &code, int maxDelivery) {
    name_ = name;
    id_ = id;
    code_ = code;
    municipality_ = municipality;
    maxDelivery_ = maxDelivery;
}

std::string Reservoir::getName(){
    return name_;
}

std::string Reservoir::getId(){
    return id_;
}

std::string Reservoir::getCode(){
    return code_;
}

std::string Reservoir::getMunicipality(){
    return municipality_;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery_;
}

Reservoir::Reservoir() = default;


