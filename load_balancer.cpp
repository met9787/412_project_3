/**
 * @file load_balancer.cpp
 * @brief Implements the LoadBalancer class (router and worker modes).
 */
#include "load_balancer.h"
#include "request.h"

#include <cstdlib>
#include <sstream>

using std::stringstream;

LoadBalancer::LoadBalancer(LBMode mode, int num_servers) : mode(mode) {
    current_time = 0;
    processing_lb = nullptr;
    streaming_lb = nullptr;

    lower_threshold = num_servers / 2;
    upper_threshold = num_servers * 5;
    min_servers = 1;
    max_servers = num_servers * 4;

    for (int i = 0; i < num_servers; i++) {
        servers.push_back(WebServer());
    }

    initial_num_servers = static_cast<int>(servers.size());
    initial_queue_size = static_cast<int>(request_q.size());
    servers_added = 0;
    servers_removed = 0;
}

LoadBalancer::LoadBalancer(LoadBalancer* processing, LoadBalancer* streaming, int initial_requests)
    : mode(LBMode::ROUTER), processing_lb(processing), streaming_lb(streaming) {
    current_time = 0;
    upper_threshold = 0;
    lower_threshold = 0;
    min_servers = 0;
    max_servers = 0;

    for (int i = 0; i < initial_requests; i++) {
        request_q.push(create_request());
    }

    initial_num_servers = static_cast<int>(servers.size());
    initial_queue_size = static_cast<int>(request_q.size());
    servers_added = 0;
    servers_removed = 0;
}

void LoadBalancer::add_request(Request r) {
    request_q.push(r);
}

void LoadBalancer::scale() {
    int qsize = static_cast<int>(request_q.size());

    if (qsize > upper_threshold && static_cast<int>(servers.size()) < max_servers) {
        servers.push_back(WebServer());
        servers_added++;
        return;
    }

    if (qsize < lower_threshold && static_cast<int>(servers.size()) > min_servers) {
        for (size_t i = 0; i < servers.size(); i++) {
            if (!servers[i].busy()) {
                servers.erase(servers.begin() + i);
                servers_removed++;
                break;
            }
        }
    }
}

void LoadBalancer::cycle() {
    current_time++;

    if (mode == LBMode::ROUTER) {
        while (!request_q.empty()) {
            Request r = request_q.front();
            request_q.pop();

            if (r.job_type == 'p') {
                processing_lb->add_request(r);
            } else {
                streaming_lb->add_request(r);
            }
        }

        for (int j = 0; j < rand() % 5; j++) {
            add_request(create_request());
        }

        return;
    }

    for (size_t i = 0; i < servers.size(); i++) {
        if (!servers[i].busy()) {
            if (!request_q.empty()) {
                servers[i].assign_request(request_q.front());
                request_q.pop();
            }
        } else {
            servers[i].cycle();
        }
    }

    scale();
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

int LoadBalancer::get_time() const {
    return current_time;
}

int LoadBalancer::get_queue_size() const {
    return static_cast<int>(request_q.size());
}

int LoadBalancer::get_num_servers() const {
    return static_cast<int>(servers.size());
}

int LoadBalancer::get_busy_servers() const {
    int busy_count = 0;
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].busy()) {
            busy_count++;
        }
    }
    return busy_count;
}

void LoadBalancer::log_status(ostream& out, const string& label) const {
    out << "time=" << current_time
        << " label=" << label
        << " queue=" << get_queue_size()
        << " busy=" << get_busy_servers()
        << "/" << get_num_servers()
        << "\n";
}

void LoadBalancer::log_summary(ostream& out, const string& label) const {
    out << "=== summary: " << label << " ===\n"
        << "starting queue size: " << initial_queue_size << "\n"
        << "ending queue size:   " << get_queue_size() << "\n"
        << "starting web servers: " << initial_num_servers << "\n"
        << "ending web servers:   " << get_num_servers() << "\n"
        << "total web servers added:   " << servers_added << "\n"
        << "total web servers deleted: " << servers_removed << "\n";
}
