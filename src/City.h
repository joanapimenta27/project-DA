#ifndef PROJECT_DA_CITY_H
#define PROJECT_DA_CITY_H

#include <string>
#include <iostream>

class City{
private:
    std::string name_;
    std::string id_;
    std::string code_;
    float demand_{};
    int population_{};

public:
    City();

    City(const std::string &name, const std::string &id, const std::string &code, float demand,
         int population);

    std::string getName();

    std::string getId();

    std::string getCode();

    float getDemand() const;

    int getPopulation() const;
};

#endif //PROJECT_DA_CITY_H
