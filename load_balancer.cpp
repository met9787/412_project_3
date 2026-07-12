#include "load_balancer.h"

#include <cstdlib>
#include <ctime>
#include <iostream>

using std::queue;

LoadBalancer::LoadBalancer(int num_servers, int t, queue<Request> q) {
    threshold = t;
    request_q = q;

    for (int i = 0; i < num_servers; i++) {
        servers.push_back(WebServer());
    }
}

void LoadBalancer::cycle() {
    for (int i = 0; i < servers.size(); i++) {

        if (!servers[i].busy()) {

            if (!request_q.empty()) {
                servers[i].assign_request(request_q.front());
                request_q.pop();
            }

        } else {
            servers[i].cycle();
        }
    }
    return;
}



