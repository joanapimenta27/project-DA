#include "Management.h"

Management::Management(int dataSet) : reservoirs_(std::make_unique<std::unordered_map<std::string, Reservoir>>()),
                                      stations_(std::make_unique<std::unordered_map<std::string, Station>>()),
                                      cities_(std::make_unique<std::unordered_map<std::string, City>>()),
                                      waterNetwork_(std::make_unique<Graph<std::string>>())
{

    FileReader reservoirsData("../Data/SmallDataSet/Reservoirs_Madeira.csv");
    FileReader stationsData("../Data/SmallDataSet/Stations_Madeira.csv");
    FileReader citiesData("../Data/SmallDataSet/Cities_Madeira.csv");
    FileReader pipesData("../Data/SmallDataSet/Pipes_Madeira.csv");


    if (dataSet){
        reservoirsData = FileReader("../Data/BigDataSet/Reservoir.csv");
        stationsData = FileReader("../Data/BigDataSet/Stations.csv");
        citiesData = FileReader("../Data/BigDataSet/Cities.csv");
        pipesData = FileReader("../Data/BigDataSet/Pipes.csv");
    }

    for (std::vector<std::string> line : reservoirsData.getData()){
        reservoirs_->insert({line.at(3), Reservoir(line.at(0), line.at(1), line.at(2), line.at(3), std::stoi(line.at(4)))});
    }
    for (std::vector<std::string> line : stationsData.getData()){
        stations_->insert({line.at(1), Station(line.at(0), line.at(1))});
    }
    for (std::vector<std::string> line : citiesData.getData()){
        cities_->insert({line.at(2), City(line.at(0), line.at(1), line.at(2), std::stof(line.at(3)), std::stoi(line.at(4)))});
    }

    for (const auto& reservoir : *reservoirs_){
        waterNetwork_->addVertex(reservoir.first);
    }
    for (const auto& station : *stations_){
        waterNetwork_->addVertex(station.first);
    }
    for (const auto& city : *cities_){
        waterNetwork_->addVertex(city.first);
    }

    for (const std::vector<std::string> &line : pipesData.getData()){
        std::string pointA = line.at(0);
        std::string pointB = line.at(1);
        double capacity = std::stod(line.at(2));
        int direction = std::stoi(line.at(3));

        if (direction){
            waterNetwork_->addEdge(pointA, pointB, capacity);
        }
        else{
            waterNetwork_->addBidirectionalEdge(pointA, pointB, capacity);
        }
    }
}



const std::unordered_map<std::string, City> &Management::getCities(){
    return *cities_;
}

const std::unique_ptr<std::unordered_map<std::string, Reservoir>> &Management::getReservoirs() const {
    return reservoirs_;
}

const std::unique_ptr<Graph<std::string>> &Management::getWaterNetwork() const {
    return waterNetwork_;
}

const std::vector<std::vector<std::vector<std::string>>> &Management::getFlowPaths() const {
    return flowPaths_;
}

const std::unordered_map<std::string, std::string> &Management::getEdgesFlow() const {
    return edgesFlow_;
}

const std::unique_ptr<std::unordered_map<std::string, Station>> &Management::getStations() const {
    return stations_;
}

Reservoir Management::getReservoirByCode(const std::string &code){
    auto it = reservoirs_->find(code);
    if (it == reservoirs_->end()) return {};
    return it->second;
}

Station Management::getStationByCode(const std::string &code){
    auto it = stations_->find(code);
    if (it == stations_->end()) return {};
    return it->second;
}

City Management::getCityByCode(const std::string &code){
    auto it = cities_->find(code);
    if (it == cities_->end()) return {};
    return it->second;
}


double Management::maxFlow(const Graph<std::string>& g, const std::string& code){

    std::vector<std::vector<std::string>> vector_of_paths;

    Graph<std::string> max_flow=g;

    max_flow.addVertex("sink");
    max_flow.addVertex("source");
    Vertex<std::string> *source=max_flow.findVertex("source");
    Vertex<std::string> *sink=max_flow.findVertex("sink");

    for(Vertex<std::string> *v : max_flow.getVertexSet()){
        if(v->getInfo().substr(0,1)=="R"){
            for(const auto& r:*reservoirs_){
                if(r.first == v->getInfo()){
                    source->addEdge(v,r.second.getMaxDelivery());
                }
            }

        }
        if(v->getInfo().substr(0,1)=="C"){
            for(const auto& c:*cities_){
                if(c.first == v->getInfo()){
                    v->addEdge(sink,c.second.getDemand());
                }
            }
        }

    }

    for(Vertex<std::string> *v:max_flow.getVertexSet()){
        for(Edge<std::string> *e:v->getAdj()){
            e->setFlow(0);
        }

    }

    while(augmentationPathFinder(&max_flow,source,sink)){
        std::vector<double> val;
        std::vector<std::vector<std::string>> path;
        double mini=INF;

        for(Vertex<std::string> *v=sink; v!=source;){
            Edge<std::string> *e=v->getPath();
            if(v==e->getDest()){
                mini=std::min(mini,e->getWeight() - e->getFlow());
                v=e->getOrig();
            }
            else{
                mini=std::min(mini,e->getFlow());
                v=e->getDest();
            }
        }
        for(Vertex<std::string> *v=sink; v!=source;){
            std::vector<std::string> step;
            Edge<std::string> *e=v->getPath();
            double flow=e->getFlow();
            if(v==e->getDest()){
                e->setFlow(flow+mini);
                v=e->getOrig();
                step.push_back(e->getDest()->getInfo());
                step.push_back(e->getOrig()->getInfo());
            }
            else{
                e->setFlow(flow-mini);
                v=e->getDest();
                step.push_back(e->getOrig()->getInfo());
                step.push_back(e->getDest()->getInfo());
            }

            step.push_back(std::to_string(e->getFlow()));
            path.push_back(step);
        }
        flowPaths_.push_back(path);
    }

    for (auto v : waterNetwork_->getVertexSet()){
        for (auto e : v->getAdj()){
            Vertex<std::string> *dest = e->getDest();
            edgesFlow_.insert({key(v->getInfo(), dest->getInfo()), std::to_string(e->getFlow())});
        }
    }

    double res=0;
    for(Edge<std::string> *e: max_flow.findVertex(code)->getIncoming()){
        res+=e->getFlow();
    }
    return res;
}


std::vector<std::vector<std::string>> Management::checkWaterNeeds() {
    std::vector<std::vector<std::string>> result;
    for (const auto& city : *cities_) {
        float waterNeeded = city.second.getDemand();
        double waterDelivered = 0;

        for (auto e : waterNetwork_->findVertex(city.first)->getIncoming()){
            waterDelivered += std::stod(edgesFlow_[key(e->getOrig()->getInfo(), e->getDest()->getInfo())]);
        }

        std::cout << city.first << " " << waterDelivered << std::endl;

        if (waterDelivered < waterNeeded) {
            std::vector<std::string> v;
            v.emplace_back(city.first);
            v.emplace_back(std::to_string(waterNeeded - waterDelivered));
            v.emplace_back(city.second.getName());
            v.emplace_back(std::to_string(waterNeeded));
            result.emplace_back(v);

        }
    }
    return result;
}


std::unordered_map<std::string, std::string> Management::checkWaterNeedsReservoir(const std::vector<std::wstring>& reservoirs){

    std::unordered_map<std::string, std::string> res;

    Graph<std::string> g = createGraphCopy(*waterNetwork_);

    for (const auto& ws : reservoirs){
        g.removeVertex(converter1.to_bytes(ws));
    }
    maxFlow(g, "sink");
    for (const auto& city : *cities_){
        double val = 0;
        for(Edge<std::string> *e: g.findVertex(city.first)->getIncoming()){
            val += e->getFlow();
        }
        res.insert({city.first, std::to_string(val)});
    }
    return res;
}

std::unordered_map<std::string, std::string> Management::checkWaterNeedsPumps(const std::vector<std::wstring>& pumps){

    std::unordered_map<std::string, std::string> res;

    Graph<std::string> g = createGraphCopy(*waterNetwork_);

    for (const auto& ws : pumps){
        g.removeVertex(converter1.to_bytes(ws));
    }
    maxFlow(g, "sink");
    for (const auto& city : *cities_){
        double val = 0;
        for(Edge<std::string> *e: g.findVertex(city.first)->getIncoming()){
            val += e->getFlow();
        }
        res.insert({city.first, std::to_string(val)});
    }
    return res;
}

std::vector<std::pair<std::string, std::vector<std::pair<std::string, int>>>> Management::checkWaterNeedsWithFailures() {
    std::vector<std::pair<std::string, std::vector<std::pair<std::string, int>>>> result;
    for (const auto& city : *cities_) {
        int waterNeeded = city.second.getDemand();
        int waterDelivered = 0;
        for (const auto& edge : waterNetwork_->findVertex(city.first)->getAdj()) {
            waterDelivered += edge->getWeight();
        }
        if (waterDelivered < waterNeeded) {
            std::vector<std::pair<std::string, int>> affectedCities;
            for (const auto& edge : waterNetwork_->findVertex(city.first)->getAdj()) {
                if (edge->getWeight() > 0) {  // Check for pipelines currently carrying water
                    waterDelivered -= edge->getWeight();  // Simulate failure by subtracting the pipeline's flow capacity
                    if (waterDelivered < waterNeeded) {
                        affectedCities.emplace_back(edge->getDest()->getInfo(), waterNeeded - waterDelivered);
                    }
                    waterDelivered += edge->getWeight();  // Restore waterDelivered for the next iteration
                }
            }
            result.emplace_back(city.first, affectedCities);

        }
    }
    return result;
}



