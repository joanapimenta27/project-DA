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

        if (std::stoi(line.at(3))){
            waterNetwork_->addEdge(pointA, pointB, std::stoi(line.at(2)));
        }
        else{
            waterNetwork_->addBidirectionalEdge(pointA, pointB, std::stoi(line.at(2)));
        }
    }
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

Graph<std::string> maxFlow(Graph<std::string> g){
    Graph<std::string> max_flow=g;
    max_flow.addVertex("sink");
    max_flow.addVertex("source");
    for(Vertex<std::string> *v : max_flow.getVertexSet()){
        if(v->getInfo().substr(0,1)=="R"){
            max_flow.findVertex("source")->addEdge(v,INT_MAX);
        }
        if(v->getInfo().substr(0,1)=="C"){
            v->addEdge(max_flow.findVertex("sink"),INT_MAX);
        }
    }

    for(Vertex<std::string> *v:max_flow.getVertexSet()){
        for(Edge<std::string> *e:v->getAdj()){
            e->setFlow(0);
        }
    }

    while(augmentationPathFinder(&max_flow,max_flow.findVertex("source"),max_flow.findVertex("sink"))){
        double mini=INF;

        for(Vertex<std::string> *v=max_flow.findVertex("sink"); v!=max_flow.findVertex("source");){
            Edge<std::string> *e=v->getPath();
            if(v==e->getDest()){
                mini=std::min(mini,e->getWeight()-e->getFlow());
                v=e->getOrig();
            }
            else{
                mini=std::min(mini,e->getFlow());
                v=e->getDest();
            }
        }

        for(Vertex<std::string> *v=max_flow.findVertex("sink"); v!=max_flow.findVertex("source");){
            Edge<std::string> *e=v->getPath();
            double flow=e->getFlow();
            if(v==e->getDest()){
                e->setFlow(flow+mini);
                v=e->getOrig();
            }
            else{
                e->setFlow(flow-mini);
                v=e->getDest();
            }
        }

        return max_flow;

    }





}



