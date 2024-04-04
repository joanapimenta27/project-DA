#ifndef PROJECT_DA_STATION_H
#define PROJECT_DA_STATION_H

#include<iostream>
#include<string>

class Station{
private:
    std::string id_;
    std::string code_;

public:
    Station();

    Station(const std::string &id, const std::string &code);

    std::string getCode();

    std::string getId();
};

#endif //PROJECT_DA_STATION_H
