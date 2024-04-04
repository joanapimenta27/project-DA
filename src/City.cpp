#include "City.h"

City::City(const std::string &name, const std::string &id, const std::string &code, float demand, int population) {
    name_ = name;
    id_ = id;
    code_ = code;
    demand_ = demand;
    population_ = population;
}

std::string City::getName() {
    return name_;
}

std::string City::getId() {
    return id_;
}

std::string City::getCode(){
    return code_;
}

float City::getDemand() const{
    return demand_;
}

int City::getPopulation() const{
    return population_;
}

City::City() = default;