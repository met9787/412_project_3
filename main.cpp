#include <iostream>

#include "load_balancer.h"

using std::cout, std::cin;

int main() {

    int num_servers;
    int num_cycles;

    cout << "Number of Servers: ";
    cin >> num_servers;
    cout << "\nNumber of Cycles: ";
    cin >> num_cycles;
    cout << "\n";

    LoadBalancer load_balancer(num_servers);
    load_balancer.run(num_cycles);
}
