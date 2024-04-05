//
// Created by master on 02-04-2024.
//

#ifndef PROJECT_DA_MANAGEMENT_H
#define PROJECT_DA_MANAGEMENT_H

#include <vector>
#include <algorithm>
#include <memory>
#include <set>
#include <cmath>
#include <unordered_map>
#include <variant>
#include "FileReader.h"
#include "Reservoir.h"
#include "Station.h"
#include "City.h"
#include "../data_structures/Graph.h"

class Management {
private:
    std::unique_ptr<std::unordered_map<std::string, Reservoir>> reservoirs_;
    std::unique_ptr<std::unordered_map<std::string, Station>> stations_;
    std::unique_ptr<std::unordered_map<std::string, City>> cities_;
    std::unique_ptr<Graph<std::string>> waterNetwork_;

public:
    Management(int dataSet);

    Reservoir getReservoirByCode(const std::string &code);

    Station getStationByCode(const std::string &code);

    City getCityByCode(const std::string &code);

    const std::unordered_map<std::string, City> &getCities();
};


#endif //PROJECT_DA_MANAGEMENT_H
