//
// Created by joana on 05/04/24.
//
#include "utils.h"

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

            visitAndTest(q,e,e->getDest(),e->getWeight()-e->getFlow());

        }
        for(Edge<std::string> *e: f->getIncoming()){

            visitAndTest(q,e,e->getOrig(),e->getFlow());
        }

    }
    return sink->isVisited();
}