

#ifndef PROJECT_DA_MANAGEMENT_H
#define PROJECT_DA_MANAGEMENT_H

#include <vector>
#include <algorithm>
#include <memory>
#include <set>
#include <cmath>
#include <unordered_map>
#include <variant>
#include <climits>
#include <codecvt>
#include <locale>
#include "utils.h"
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
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

public:
    Management(int dataSet);

    Reservoir getReservoirByCode(const std::string &code);

    Station getStationByCode(const std::string &code);

    City getCityByCode(const std::string &code);

    const std::unordered_map<std::string, City> &getCities();

    double maxFlow(const Graph<std::string>& g, std::string code);

    const std::unique_ptr<Graph<std::string>> &getWaterNetwork() const;
};


#endif //PROJECT_DA_MANAGEMENT_H
