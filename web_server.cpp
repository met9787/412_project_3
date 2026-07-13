/**
 * @file web_server.cpp
 * @brief Implements the WebServer class.
 */
#include "web_server.h"

WebServer::WebServer() {
    is_busy = false;
};

bool WebServer::busy() const {
    return is_busy;
}

void WebServer::assign_request(Request r) {
    curr_request = r;
    is_busy = true;
}

int WebServer::get_time_remaining() {
    return curr_request.time_remaining;
}

void WebServer::cycle() {
    if (!is_busy) { return; }

    curr_request.time_remaining--;
    if (curr_request.time_remaining <= 0) {
        is_busy = false;
    }

    return;

}
