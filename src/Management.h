

#ifndef PROJECT_DA_MANAGEMENT_H
#define PROJECT_DA_MANAGEMENT_H

#include <vector>
#include <algorithm>
#include <memory>
#include <set>
#include <cmath>
#include <map>
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


struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
        auto h1 = std::hash<T1>{}(p.first);
        auto h2 = std::hash<T2>{}(p.second);
        return h1 ^ h2;
    }
};



class Management {
private:

    std::unique_ptr<std::unordered_map<std::string, Reservoir>> reservoirs_;
    std::unique_ptr<std::unordered_map<std::string, Station>> stations_;
    std::unique_ptr<std::unordered_map<std::string, City>> cities_;
    std::unique_ptr<Graph<std::string>> waterNetwork_;
    std::vector<std::vector<std::vector<std::string>>> flowPaths_;
    std::unordered_map<std::string , std::string> edgesFlow_;

public:
    Management(int dataSet);

    Reservoir getReservoirByCode(const std::string &code);

    Station getStationByCode(const std::string &code);

    City getCityByCode(const std::string &code);

    const std::unordered_map<std::string, City> &getCities();

    std::vector<std::vector<std::string>> checkWaterNeeds();

    double maxFlow(const Graph<std::string>& g, const std::string& code);

    const std::unique_ptr<Graph<std::string>> &getWaterNetwork() const;

    const std::unordered_map<std::string, std::string> &getEdgesFlow() const;

    const std::unique_ptr<std::unordered_map<std::string, Reservoir>> &getReservoirs() const;

    std::unordered_map<std::string, std::string> checkWaterNeedsReservoir(const std::vector<std::wstring> &reservoirs);

    const std::vector<std::vector<std::vector<std::string>>> &getFlowPaths() const;

    std::unordered_map<std::string, std::string> checkWaterNeedsPumps(const std::vector<std::wstring> &pumps);

    const std::unique_ptr<std::unordered_map<std::string, Station>> &getStations() const;

    std::unordered_map<std::string, std::string> checkWaterNeedsPipes(const std::vector<std::wstring> &pumps);
};


#endif //PROJECT_DA_MANAGEMENT_H
