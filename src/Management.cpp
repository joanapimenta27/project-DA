
#include "Management.h"

/**
 * @brief Constructor for the Management class.
 *
 * This constructor initializes a Management object based on the specified dataset.
 *
 * @details The time complexity of this constructor depends on the size of the dataset:
 *
 *          - For the small dataset:
 *
 *            - If there are n reservoirs, m stations, and k cities, the time complexity is O(n + m + k).
 *            - If there are p pipes, the time complexity of adding edges to the graph is O(p).
 *          - For the big dataset:
 *            - If there are n reservoirs, m stations, and k cities, the time complexity is O(n + m + k).
 *            - If there are p pipes, the time complexity of adding edges to the graph is O(p).
 *
 * @param dataSet Flag indicating whether the dataset used is small or big.
 *                If 0, small dataset is used, otherwise big dataset is used.
 */
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

        pipes_.insert({key(pointA, pointB), capacity});
    }
}

/**
 * @brief Get a reference to the cities map.
 *
 * @return Reference to the cities map.
 */
const std::unordered_map<std::string, City> &Management::getCities(){
    return *cities_;
}

/**
 * @brief Get a constant reference to the reservoirs map.
 *
 * @return Constant reference to the reservoirs map.
 */
const std::unique_ptr<std::unordered_map<std::string, Reservoir>> &Management::getReservoirs() const {
    return reservoirs_;
}

/**
 * @brief Get a constant reference to the water network graph.
 *
 * @return Constant reference to the water network graph.
 */
const std::unique_ptr<Graph<std::string>> &Management::getWaterNetwork() const {
    return waterNetwork_;
}

/**
 * @brief Get a constant reference to the flow paths.
 *
 * @return Constant reference to the flow paths.
 */
const std::vector<std::vector<std::vector<std::string>>> &Management::getFlowPaths() const {
    return flowPaths_;
}

/**
 * @brief Get a constant reference to the edges flow.
 *
 * @return Constant reference to the edges flow.
 */
const std::unordered_map<std::string, std::string> &Management::getEdgesFlow() const {
    return edgesFlow_;
}

/**
 * @brief Get a constant reference to the stations map.
 *
 * @return Constant reference to the stations map.
 */
const std::unique_ptr<std::unordered_map<std::string, Station>> &Management::getStations() const {
    return stations_;
}

/**
 * @brief Get the reservoir object by its code.
 *
 * This function retrieves the reservoir object associated with the given code from the map of reservoirs.
 * If the code is not found in the map, it returns an empty object.
 *
 * @param code The code of the reservoir.
 * @return Reservoir object corresponding to the code, or an empty object if not found.
 *
 * @complexity O(1) on average for a hash map lookup.
 */
Reservoir Management::getReservoirByCode(const std::string &code){
    auto it = reservoirs_->find(code);
    if (it == reservoirs_->end()) return {};
    return it->second;
}

/**
 * @brief Get the station object by its code.
 *
 * This function retrieves the station object associated with the given code from the map of stations.
 * If the code is not found in the map, it returns an empty  object.
 *
 * @param code The code of the station.
 * @return Station object corresponding to the code, or an empty object if not found.
 *
 * @complexity O(1) on average for a hash map lookup.
 */
Station Management::getStationByCode(const std::string &code){
    auto it = stations_->find(code);
    if (it == stations_->end()) return {};
    return it->second;
}

/**
 * @brief Get the city object by its code.
 *
 * This function retrieves the city object associated with the given code from the map of cities.
 * If the code is not found in the map, it returns an empty object.
 *
 * @param code The code of the city.
 * @return City object corresponding to the code, or an empty object if not found.
 *
 * @complexity O(1) on average for a hash map lookup.
 */
City Management::getCityByCode(const std::string &code){
    auto it = cities_->find(code);
    if (it == cities_->end()) return {};
    return it->second;
}

/**
 * @brief Computes the maximum flow in the water network graph.
 *
 * This function calculates the maximum flow in the given water network graph using the
 * Edmonds-Karp algorithm . It adds a source and a sink vertex to the graph and connects them to the
 * reservoirs and cities, respectively. Then, it iteratively finds augmentation paths from the source to the sink
 * using BFS until no more paths can be found. During each iteration, it updates the flow along the found paths.
 * Finally, it retrieves the maximum flow value from the incoming edges of the target vertex specified by the code.
 *
 * @param g The water network graph.
 * @param code The code representing the target of the maximum flow computation.
 * @return The maximum flow value.
 *
 * @complexity O(V * E^2), where V is the number of vertices in the graph and E is the number of edges.
 */
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

/**
 * @brief Checks water needs for all cities and computes any deficits.
 *
 * This function calculates the water needs for all cities in the water network
 * and computes any deficits. It iterates through each city, retrieves its demand,
 * calculates the total water delivered to the city from incoming edges in the water network graph,
 * and compares it with the demand. If the delivered water is less than the demand,
 * information about the city with the deficit is added to the result vector.
 *
 * @return A vector containing information about cities with water deficits.
 *
 * @complexity O(N + E), where N is the number of vertices (cities) and E is the number of edges (pipes)
 */
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

/**
 * @brief Checks water needs for cities after removing certain reservoirs.
 *
 * This function calculates the water needs for cities after removing specified reservoirs
 * from the water network. It creates a copy of the water network graph,
 * removes the specified reservoirs, calculates the maximum flow in the modified graph,
 * and then computes the water needs for each city based on the incoming flow to
 * their corresponding vertices in the graph.
 *
 * @param reservoirs Vector containing the reservoirs to be removed.
 * @return Map containing the cities and their water needs after reservoir removal.
 *
 * @complexity O(N + E), where N is the number of vertices (cities) and E is the number of edges (pipes)
 */
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
    std::cout << res["C_4"];
    return res;
}

/**
 * @brief Checks water needs for cities after removing certain pumps.
 *
 * This function calculates the water needs for cities after removing specified pumps
 * from the water network. It creates a copy of the water network graph,
 * removes the specified pumps, calculates the maximum flow in the modified graph,
 * and then computes the water needs for each city based on the incoming flow to
 * their corresponding vertices in the graph.
 *
 * @param pumps Vector containing the pumps to be removed.
 * @return Map containing the cities and their water needs after pump removal.
 *
 * @complexity O(N + E), where N is the number of vertices (cities) and E is the number of edges (pipes)
 */
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

/**
 * @brief Checks water needs for cities after removing certain pipes.
 *
 * This function calculates the water needs for cities after removing specified pipes
 * from the water network. It creates a copy of the water network graph,
 * removes the specified pipes, calculates the maximum flow in the modified graph,
 * and then computes the water needs for each city based on the incoming flow to
 * their corresponding vertices in the graph.
 *
 * @param pipes Vector containing the pipes to be removed.
 * @return Map containing the cities and their water needs after pipe removal.
 *
 * @complexity O(N + E), where N is the number of vertices (cities) and E is the number of edges (pipes)
 */
std::unordered_map<std::string, std::string> Management::checkWaterNeedsPipes(const std::vector<std::wstring>& pumps){

    std::unordered_map<std::string, std::string> res;

    Graph<std::string> g = createGraphCopy(*waterNetwork_);

    for (const auto& ws : pumps){
        std::string dest = stringDivider(ws, 0, '|');
        std::string orig = stringDivider(ws, 1, '|');
        g.removeEdge(orig, dest);
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

void Management::balanceBasicMetrics (const Graph<std::string>& g){
    double sum_diff = 0.0;
    double sum_diff_squared = 0.0;
    double max_diff = 0.0;
    std::string d;
    std::string o;

    for (const auto& pair : pipes_) {
        o = stringDivider(converter.from_bytes(pair.first), 0, '|');
        d = stringDivider(converter.from_bytes(pair.first), 1, '|');
        for (auto e : g.findVertex(o)->getAdj()){
            if (e->getDest()->getInfo() == d){
                double diff = pair.second - e->getFlow();
                sum_diff += diff;
                sum_diff_squared += diff * diff;
                max_diff = std::max(max_diff, std::abs(diff));
            }
        }

    }

    double average_diff = sum_diff / pipes_.size();
    double variance_diff = sum_diff_squared / pipes_.size() - (average_diff * average_diff);

    std::cout << "Initial Metrics:" << std::endl;
    std::cout << "Average Difference: " << average_diff << std::endl;
    std::cout << "Variance of Difference: " << variance_diff << std::endl;
    std::cout << "Maximum Difference: " << max_diff << std::endl;
}

Graph<std::string> Management::balance(const Graph<std::string>& g){
    Graph<std::string> a = createGraphCopy(g);
    maxFlow(a, "source");
    //Logica que procura as bifurcaçoes, analiza a que tem mais espaço e redireciona mais agua
    return a;
}




