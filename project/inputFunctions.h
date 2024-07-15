#ifndef INPUTFUNCTIONS_H
#define INPUTFUNCTIONS_H

#include <string>
#include <iostream>
#include <sstream>
#include <regex>
#include <vector>
#include "graph.h"

void readVertixNumberInput(Graph *&g, const std::string &line);
bool readEdgeInput(Graph *&g, const std::string &line);

#endif
