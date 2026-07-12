#pragma once

#include "request.h"
#include "web_server.h"

#include <queue>
#include <vector>

using std::queue, std::vector;

class LoadBalancer {
    private:
        int threshold;
        queue<Request> request_q;
        vector<WebServer> servers;
        
    public:
        LoadBalancer(int num_servers, int t, queue<Request> q);
        void cycle();

};


