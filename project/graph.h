#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include "outputFunctions.h"

class Graph
{
public:
    Graph(int vertixNumber);
    ~Graph();
    int getVertixNumber();
    bool getEdgeCommandGiven();
    void setEdgeCommandGiven(bool edgeCommandGiven);
    void addEdge(int v, int w);
    std::vector<std::pair<int, int>> getEdges();
    std::vector<int> approximateMinimumVertexCover1();
    std::vector<int> approximateMinimumVertexCover2();
    static double approx_1_size;
    static double approx_2_size;
    static double approx_vc1_runtime;
    static double approx_vc2_runtime;
    static std::string output_v1;
    // void printApproximateMinVertexCover1();
    // void printApproximateMinVertexCover2();

private : int vertixNumber;
    bool edgeCommandGiven;
    std::vector<std::vector<int>> neighbourVertexArray;
    
};

#endif