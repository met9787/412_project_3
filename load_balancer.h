#pragma once

#include "request.h"
#include "web_server.h"

#include <queue>
#include <vector>

using std::queue, std::vector;

class LoadBalancer {
    private:
        int upper_threshold;
        int lower_threshold;
        queue<Request> request_q;
        vector<WebServer> servers;
        
    public:
        LoadBalancer(int num_servers);
        void cycle();
        Request create_request();        
        void run(int num_cycles);
};


