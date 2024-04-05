#ifndef PROJECT_DA_RESERVOIR_H
#define PROJECT_DA_RESERVOIR_H

#include <iostream>
#include <string>
#include <unordered_map>

class Reservoir {
private:
    std::string name_;
    std::string municipality_;
    std::string id_;
    std::string code_;
    int maxDelivery_;

public:
    Reservoir();

    Reservoir(const std::string &name, const std::string &municipality, const std::string &id, const std::string &code, int maxDelivery);

    int getMaxDelivery() const;

    std::string getId();

    std::string getName();

    std::string getCode();

    std::string getMunicipality();
};

#endif //PROJECT_DA_RESERVOIR_H
