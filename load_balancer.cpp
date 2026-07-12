#include "load_balancer.h"
#include "request.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>

using std::queue, std::stringstream;

LoadBalancer::LoadBalancer(int num_servers) {    

    for (int i = 0; i < 100 * num_servers; i++) {
        request_q.push(create_request());
    }

    upper_threshold = request_q.size() * 2;
    lower_threshold = request_q.size() / 2;

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

Request LoadBalancer::create_request() {

    stringstream ip1;
    ip1 << rand() % 256 << "." << rand() % 256 << "." << rand() % 256 << "." << rand() % 256;
    string ip_1 = ip1.str();
    stringstream ip2;
    ip2 << rand() % 256 << "." << rand() % 256 << "." << rand() % 256 << "." << rand() % 256;
    string ip_2 = ip2.str();
    
    int t = rand() % 10;
    char job;

    if (rand() % 2 == 0) {
        job = 's';
    } else {
        job = 'p';
    }

    Request r;
    r.ip_in = ip_1;
    r.ip_out = ip_2;
    r.time_remaining = t;
    r.job_type = job;

    return r;
}

void LoadBalancer::run(int num_cycles) {
    for (int i = 0; i < num_cycles; i++) {
        cycle();

        for (int j = 0; j < rand() % 5; j++) {
            request_q.push(create_request());
        }

    }
}







