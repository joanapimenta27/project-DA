//
// Created by joana on 05/04/24.
//
#include "utils.h"

std::wstring_convert<std::codecvt_utf8<wchar_t>> converter1;

std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

Graph<std::string> createGraphCopy(const Graph<std::string>& og_graph){
    Graph<std::string> copyOfGraph;
    for (const auto& v : og_graph.getVertexSet()){
        copyOfGraph.addVertex(v->getInfo());
    }
    for (const auto& v : og_graph.getVertexSet()){
        for (const auto& e : v->getAdj()){
            copyOfGraph.addEdge(e->getOrig()->getInfo(), e->getDest()->getInfo(), e->getWeight());
        }
    }
    return copyOfGraph;
}

std::string key(const std::string& i,const std::string& j) {return i + j;}

double findMin(const std::vector<double>& values) {
    if (values.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    auto minElement = std::min_element(values.begin(), values.end());

    return *minElement;
}

double calcPathMin(const std::vector<std::vector<std::string>> &path){
    std::vector<double> minList;
    for (auto s : path){
        minList.push_back(std::stod(s[2]));
    }
    return findMin(minList);
}

void visitAndTest(std::queue<Vertex<std::string>*> &q,Edge<std::string> *e,Vertex<std::string> *d, double spare){
    if(spare>0 && !d->isVisited() ){
        d->setVisited(true);
        d->setPath(e);
        q.push(d);
    }
}
bool augmentationPathFinder(Graph<std::string> *g ,Vertex<std::string> *source,Vertex<std::string> *sink){
    std::queue<Vertex<std::string> *> q;
    for(Vertex<std::string> *v : g->getVertexSet()){
        v->setVisited(false);
    }

    source->setVisited(true);
    q.push(source);

    while(!sink->isVisited() && !q.empty()){
        Vertex<std::string> *f=q.front();
        q.pop();

        for(Edge<std::string> *e: f->getAdj()){

            visitAndTest(q,e,e->getDest(),e->getWeight() - e->getFlow());

        }
        for(Edge<std::string> *e: f->getIncoming()){

            visitAndTest(q,e,e->getOrig(),e->getFlow());
        }

    }

    return sink->isVisited();
}

std::vector<std::vector<std::vector<std::string>>> flowDataLineRemover (std::vector<std::wstring> codes, std::vector<std::vector<std::vector<std::string>>> flowData_){

    std::vector<std::vector<std::vector<std::string>>> res;
    std::unordered_map<std::string, std::string> info;

    for (int idx = 0; idx < flowData_.size(); idx ++) {
        std::vector<std::vector<std::string>> path = flowData_[idx];
        for (const auto&  step: path) {
            if (std::find(codes.begin(), codes.end(), converter.from_bytes(step[0])) != codes.end() ||
                std::find(codes.begin(), codes.end(), converter.from_bytes(step[1])) != codes.end()) {
                double n = calcPathMin(path);
                info.insert({std::to_string(idx), std::to_string(n)});
                break;
            }
        }
    }
    for (int idx = 0; idx < flowData_.size(); idx++) {
        if (info.find(std::to_string(idx)) != info.end()) {
            double min_capacity = calcPathMin(flowData_[idx]);
            for (auto& step : flowData_[idx]) {
                for (int idx2 = 0; idx2 < flowData_.size(); idx2++) {
                    if (idx != idx2) {
                        for (auto& step2 : flowData_[idx2]) {
                            if (step2[0] == step[0] && step2[1] == step[1]) {
                                double new_capacity = std::stod(step2[2]) - min_capacity;
                                step2[2] = std::to_string(new_capacity);
                            }
                        }
                    }
                }
            }
        }
    }
    for (int idx = 0; idx < flowData_.size(); idx ++){
        std::vector<std::vector<std::string>> path = flowData_[idx];
        if (info.find(std::to_string(idx)) == info.end()){
            res.push_back(path);
        }
    }

    return res;
}

std::string getCityFlow (const std::string& code, const std::vector<std::vector<std::vector<std::string>>> &flowData_){
    double res = 0;

    for (const auto& path : flowData_){
        for (auto step : path)
        if (code == step[0] || code == step[1]){
            res += calcPathMin(path);
        }
    }

    return std::to_string(res);
}
