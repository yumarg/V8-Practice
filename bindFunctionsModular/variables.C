#include "variables.h"

std::map<std::string, std::string> symbolTable = {
        {"Param1", "p1"},
        {"Param2", "p2"},
        {"Param3", "p3"}
};

std::map<std::string, std::string> symbolTableCopy = symbolTable;
