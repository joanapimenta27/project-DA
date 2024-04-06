//
// Created by joana on 05/04/24.
//

#ifndef PROJECT_DA_UTILS_H
#define PROJECT_DA_UTILS_H
#include <queue>
#include <string>
#include "FileReader.h"
#include "Reservoir.h"
#include "Station.h"
#include "City.h"
#include "../data_structures/Graph.h"

void visitAndTest(std::queue<Vertex<std::string>*> &q,Edge<std::string> *e,Vertex<std::string> *d, double spare);
bool augmentationPathFinder(Graph<std::string> *g ,Vertex<std::string> *source,Vertex<std::string> *sink);

#endif //PROJECT_DA_UTILS_H
