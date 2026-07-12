#include "web_server.h"

WebServer::WebServer() {
    is_busy = false;
};

bool WebServer::busy() {
    return is_busy;
}

void WebServer::assign_request(Request r) {
    curr_request = r;
    is_busy = true;
}

int WebServer::get_time_remaining() {
    return curr_request.timeRemaining;
}

void WebServer::cycle() {
    if (!is_busy) { return; }

    curr_request.timeRemaining--;
    if (curr_request.timeRemaining <= 0) {
        is_busy = false;
    }

    return;
    
}




