#pragma once

#include "request.h"

class WebServer {
    private:
        bool is_busy;
        Request curr_request;

    public:
        WebServer();
        bool busy();
        void assign_request(Request r);
        int get_time_remaining();
        void cycle();
};




