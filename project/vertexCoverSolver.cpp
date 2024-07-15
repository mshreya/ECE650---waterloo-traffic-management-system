#include "vertexCoverSolver.h"
#include<pthread.h>

double VertexCoverSolver::cnf_sat_runtime = 0;
double VertexCoverSolver::cnf_size = 0;
bool VertexCoverSolver::cnf_timeout_flag = false;


// Constructor
VertexCoverSolver::VertexCoverSolver(Graph *g)
{
    this->g = g;
    this->literalTable.resize(g->getVertixNumber());
    this->currentVertexCoverNumber = g->getVertixNumber();
    // findMinVertexCover();
}

// Destructor
VertexCoverSolver::~VertexCoverSolver()
{
    for (size_t i = 0; i < this->literalTable.size(); ++i)
    {
        this->literalTable[i].clear();
    }
    this->literalTable.clear();
    this->g = nullptr;
}

void VertexCoverSolver::setLiteral()
{
    for (int i = 0; i < g->getVertixNumber(); ++i)
    {
        literalTable[i].clear();
        literalTable[i].resize(currentVertexCoverNumber);
        std::generate(literalTable[i].begin(), literalTable[i].end(), [&]()
                      { return Minisat::mkLit(solver->newVar()); });
    }
}

void VertexCoverSolver::setClause1()
{
    for (int i = 0; i < currentVertexCoverNumber; ++i)
    {
        Minisat::vec<Minisat::Lit> literals;
        for (const auto &row : literalTable)
        {
            literals.push(row[i]);
        }
        solver->addClause(literals);
        literals.clear();
    }
}

void VertexCoverSolver::setClause2()
{
    for (const auto &row : literalTable)
    {
        for (int i = 0; i < currentVertexCoverNumber - 1; ++i)
        {
            for (int j = i + 1; j < currentVertexCoverNumber; ++j)
            {
                solver->addClause(~row[i], ~row[j]);
            }
        }
    }
}

void VertexCoverSolver::setClause3()
{
    for (int i = 0; i < currentVertexCoverNumber; ++i)
    {
        for (int j = 0; j < g->getVertixNumber() - 1; ++j)
        {
            for (int k = j + 1; k < g->getVertixNumber(); ++k)
            {
                solver->addClause(~literalTable[j][i], ~literalTable[k][i]);
            }
        }
    }
}

void VertexCoverSolver::setClause4()
{
    for (const auto &edge : g->getEdges())
    {
        Minisat::vec<Minisat::Lit> literals;
        for (const auto &vertex : {edge.first, edge.second})
        {
            for (auto &lit : literalTable[vertex])
            {
                literals.push(lit);
            }
        }
        solver->addClause(literals);
        literals.clear();
    }
}

std::vector<int> VertexCoverSolver::getVertexCoverForCurrentVNum()
{
    solver.reset(new Minisat::Solver());
    setLiteral();
    setClause1();
    setClause2();
    setClause3();
    setClause4();

    if (solver->solve())
    {
        std::vector<int> tempVertexCover;
        for (int i = 0; i < g->getVertixNumber(); ++i)
        {
            for (int j = 0; j < currentVertexCoverNumber; ++j)
            {
                if (Minisat::toInt(solver->modelValue(literalTable[i][j])) == 0)
                {
                    tempVertexCover.push_back(i);
                }
            }
        }
        return tempVertexCover;
    }
    return {};
}

std::vector<int> VertexCoverSolver::findMinVertexCover()
{
    struct timespec ts;
    //clockid_t cid;

    // Start timer
    //pthread_getcpuclockid(pthread_self(), &cid);

    int left = 1;
    int right = g->getVertixNumber();
    std::vector<int> tempVertexCover;

    while (left <= right && !cnf_timeout_flag)
    {
        currentVertexCoverNumber = floor((left + right) / 2);
        tempVertexCover = getVertexCoverForCurrentVNum();

        if (tempVertexCover.empty())
        {
            left = currentVertexCoverNumber + 1;
        }
        else
        {
            minVertexCover.clear();
            minVertexCover = tempVertexCover;
            right = currentVertexCoverNumber - 1;
        }
    }
    //sort(minVertexCover.begin(), minVertexCover.end());
    cnf_size = minVertexCover.size();

   // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts);
    // Saves the runtime in microsecs
   // cnf_sat_runtime = (((double)ts.tv_nsec) * 0.001);

    return minVertexCover;
    // pthread_exit(NULL);
}

// void VertexCoverSolver::printCNFMinVertexCover()
// {
    
    
    

    
    
// }