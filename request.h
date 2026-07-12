#pragma once

#include <string>

using std::string;

struct Request {
    string ipIn;
    string ipOut;
    int timeRemaining;
    char jobType;
};

