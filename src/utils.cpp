//
// Created by joana on 05/04/24.
//
#include "utils.h"

/**
 * @brief Converts between wide and multibyte character strings using UTF-8 encoding.
 */
std::wstring_convert<std::codecvt_utf8<wchar_t>> converter1;

/**
 * @brief Converts between wide and multibyte character strings using UTF-8 encoding.
 */
std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

/**
 * @brief Creates a copy of the provided graph.
 *
 * This function creates a copy of the original graph by iterating through its vertexSet
 * and adding vertices and edges to the copy. It returns the copy of the original graph.
 *
 * @param og_graph Original graph to be copied.
 * @return Copy of the original graph.
 *
 * @note The time complexity of this function is O(V + E), where V is the number of vertices
 *       and E is the number of edges in the original graph.
 */
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

/**
 * @brief Divides a wide character string into two parts using the specified delimiter.
 *
 * This function takes a wide character string as input and divides it into two parts using
 * the specified delimiter character. It returns either the first or second part of the divided
 * string based on the index provided.
 *
 * @param ws Wide character string to be divided.
 * @param i Index to specify which part to return (0 for the first part, 1 for the second).
 * @param loc Delimiter character used for division.
 * @return Either the first or second part of the divided string based on the index.
 *
 * @note The time complexity of this function is O(n), where n is the length of the input string.
 */
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

/**
 * @brief Combines two strings into a single key string separated by a delimiter.
 *
 * This function takes two strings as input and combines them into a single key string
 * separated by a delimiter (|).
 *
 * @param i First part of the key.
 * @param j Second part of the key.
 * @return Combined key string.
 *
 * @note The time complexity of this function is O(m+n), where m is the size of the first string
 *       and n is the size of the second string.
 */
std::string key(const std::string& i,const std::string& j) {return i + "|" + j;}

/**
 * @brief Finds the minimum element in a vector of doubles.
 *
 * This function takes a vector of doubles as input and returns the minimum value in the vector.
 * If the input vector is empty, it returns NaN (Not a Number).
 *
 * @param values Vector of doubles.
 * @return Minimum value in the vector.
 *
 * @note The time complexity of this function is O(n), where n is the size of the input vector.
 */
double findMin(const std::vector<double>& values) {
    if (values.empty()) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    auto minElement = std::min_element(values.begin(), values.end());

    return *minElement;
}

/**
 * @brief Calculates the minimum value in a path represented as a vector of vectors of strings.
 *
 * This function iterates over each step in the path, extracts the value associated with the step,
 * converts it to a double, and adds it to a list. It then finds and returns the minimum value
 * from the list.
 *
 * @param path Vector of vectors representing the path, where each inner vector contains information about a step in the path.
 * @return Minimum value in the path.
 *
 * @note The time complexity of this function is O(n), where n is the number of steps in the path.
 */
double calcPathMin(const std::vector<std::vector<std::string>> &path){
    std::vector<double> minList;
    for (auto s : path){
        minList.push_back(std::stod(s[2]));
    }
    return findMin(minList);
}

/**
 * @brief Helper function for the augmentation path finding algorithm.
 *
 * This function is used to visit a neighbor vertex and test its eligibility for inclusion in the augmentation path.
 *
 * @param q Queue of vertices to be processed.
 * @param e Edge connecting the current vertex to its neighbor.
 * @param d Destination vertex of the edge.
 * @param spare Amount of spare capacity available on the edge.
 *
 * @note The time complexity of this function is O(1).
 */
void visitAndTest(std::queue<Vertex<std::string>*> &q,Edge<std::string> *e,Vertex<std::string> *d, double spare){
    if(spare>0 && !d->isVisited() ){
        d->setVisited(true);
        d->setPath(e);
        q.push(d);
    }
}

/**
 * @brief Finds an augmentation path from source to sink in the given graph.
 *
 * This function performs a breadth-first search to find an augmentation path from the source to the sink in the graph.
 *
 * @details The time complexity of this function depends on the size of the graph and its connectivity:
 *          - Let n be the number of vertices and m be the number of edges in the graph.
 *          - The worst-case time complexity is O(n + m) if the graph is densely connected.
 *
 * @param g Pointer to the graph.
 * @param source Pointer to the source vertex.
 * @param sink Pointer to the sink vertex.
 * @return True if an augmentation path is found, false otherwise.
 */
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
