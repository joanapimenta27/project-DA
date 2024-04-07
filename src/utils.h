//
// Created by joana on 05/04/24.
//

#ifndef PROJECT_DA_UTILS_H
#define PROJECT_DA_UTILS_H

#include <queue>
#include <string>
#include <codecvt>
#include <locale>
#include <unordered_map>
#include "Graph.h"


extern std::wstring_convert<std::codecvt_utf8<wchar_t>> converter1;
extern std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
void visitAndTest(std::queue<Vertex<std::string>*> &q,Edge<std::string> *e,Vertex<std::string> *d, double spare);
bool augmentationPathFinder(Graph<std::string> *g ,Vertex<std::string> *source,Vertex<std::string> *sink);
std::string key(const std::string& i,const std::string& j);
double findMin(const std::vector<double>& values);
double calcPathMin(const std::vector<std::vector<std::string>> &path);
Graph<std::string> createGraphCopy(const Graph<std::string>& og_graph);
std::string stringDivider(const std::wstring& s, int i, char loc);


#endif //PROJECT_DA_UTILS_H
