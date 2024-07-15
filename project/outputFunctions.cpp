#include "outputFunctions.h"

void printMinimumVertexCover(std::string title, std::vector<int> minVertexCover)
{
    int size = minVertexCover.size();
    std::sort(minVertexCover.begin(), minVertexCover.end());
    
    std::cout << title;
    for (int i = 0; i < size; ++i)
    {
        std::cout << minVertexCover[i] + 1;
        if (i < size - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << std::endl;
}