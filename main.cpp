/**
 * @file main.cpp
 * @brief Driver program for the load balancer simulation.
 *
 * Sets up a router LoadBalancer plus two worker LoadBalancers (processing
 * and streaming), runs the simulation for a user-specified number of
 * clock cycles, and writes a per-cycle status log to simulation.log.
 */
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

#include "load_balancer.h"

using std::cout, std::cin, std::ofstream;

int main() {

    srand(static_cast<unsigned int>(time(nullptr)));

    int num_servers;
    int num_cycles;

    cout << "Number of Servers: ";
    cin >> num_servers;
    cout << "\nNumber of Cycles: ";
    cin >> num_cycles;
    cout << "\n";

    int processing_servers = num_servers / 2;
    int streaming_servers = num_servers - processing_servers;
    if (processing_servers < 1) { processing_servers = 1; }
    if (streaming_servers < 1) { streaming_servers = 1; }

    LoadBalancer processing(LBMode::PROCESSING, processing_servers);
    LoadBalancer streaming(LBMode::STREAMING, streaming_servers);
    LoadBalancer router(&processing, &streaming, num_servers * 100);

    ofstream log("simulation.txt");

    for (int cycle = 0; cycle < num_cycles; cycle++) {
        router.cycle();
        processing.cycle();
        streaming.cycle();

        router.log_status(log, "router");
        processing.log_status(log, "processing");
        streaming.log_status(log, "streaming");
    }

    router.log_summary(log, "router");
    processing.log_summary(log, "processing");
    streaming.log_summary(log, "streaming");

    log.close();

    cout << "Simulation complete.\n\n";
    router.log_summary(cout, "router");
    processing.log_summary(cout, "processing");
    streaming.log_summary(cout, "streaming");
    cout << "\nSee simulation.txt for the full per-cycle log.\n";

    return 0;
}
