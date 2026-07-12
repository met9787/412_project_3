#pragma once

#include <string>

using std::string;

struct Request {
    string ip_in;
    string ip_out;
    int time_remaining;
    char job_type;
};

