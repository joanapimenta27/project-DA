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

std::string stringDivider(const std::wstring& ws, int i, char loc){
    std::string s = converter1.to_bytes(ws);
    std::string o;
    std::string d;
    bool a = true;
    for (auto c : s){
        if (c == loc){
            a = false;
            continue;
        }
        if (a){
            d.push_back(c);
        }
        else{
            o.push_back(c);
        }
    }
    if (i) {
        return d;
    }
    else{
        return o;
    }

}

std::string key(const std::string& i,const std::string& j) {return i + "|" + j;}

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
