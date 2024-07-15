#pragma once

#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <minisat/core/Solver.h>
#include "graph.h"
#include "outputFunctions.h"
#include <memory>

class VertexCoverSolver
{
public:
    VertexCoverSolver(Graph *g);
    ~VertexCoverSolver();
    // void printCNFMinVertexCover();
    std::vector<int> findMinVertexCover();
    static double cnf_sat_runtime;
    static double cnf_size;
    static bool cnf_timeout_flag;

private:
    void setClause1();
    void setClause2();
    void setClause3();
    void setClause4();
    std::vector<int> getVertexCoverForCurrentVNum();
    void setLiteral();

    int currentVertexCoverNumber;
    std::unique_ptr<Minisat::Solver> solver;
    std::vector<std::vector<Minisat::Lit>> literalTable;
    Graph *g;
    std::vector<int> minVertexCover;
};