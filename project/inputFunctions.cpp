#include "inputFunctions.h"

// Function to read vertix input command
void readVertixNumberInput(Graph *&g, const std::string &line)
{
    int vertixNumber = std::stoi(line.substr(2));
    // Check if vertixNumber is greater than 1
    if (vertixNumber < 2)
    {
        std::cerr << "Error: Number of vertices should be greater than 1.\n";
    }
    else
    {
        if (g != nullptr)
        {
            delete g;
            g = nullptr;
        }
        g = new Graph(vertixNumber);
    }
}

// Function to read edge input command
bool readEdgeInput(Graph *&g, const std::string &line)
{
    g->setEdgeCommandGiven(true);
    std::regex edgeRegex("<(\\d+),(\\d+)>");
    std::sregex_iterator edgeIterator(line.begin(), line.end(), edgeRegex);
    std::sregex_iterator endIterator;
    std::vector<std::pair<int, int>> edgeList;
    bool validEdges = true;

    // Check all edges within vertixNumber range
    while (edgeIterator != endIterator)
    {
        std::smatch match = *edgeIterator;
        int source = std::stoi(match[1]);
        int destination = std::stoi(match[2]);
        int vertixNumber = g->getVertixNumber();

        if (source >= 1 && source <= vertixNumber && destination >= 1 && destination <= vertixNumber)
        {
            edgeList.push_back({source, destination});
        }
        else
        {
            validEdges = false;
            break;
        }
        edgeIterator++;
    }

    // std::cout << edgeList.size() << std::endl;

    // Add edges if all are valid
    if (validEdges == true)
    {
        if (edgeList.empty())
        {
            std::cout << std::endl;
            validEdges = false;
        }
        for (size_t i = 0; i < edgeList.size(); i++)
        {
            const auto &edge = edgeList[i];
            int source = edge.first;
            int destination = edge.second;
            g->addEdge(source - 1, destination - 1);
        }
    }
    else
    {
        std::cerr << "Error: Input vertex is out of Graph vertices range.\n";
    }
    return validEdges;
}