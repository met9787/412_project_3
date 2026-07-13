/**
 * @file web_server.h
 * @brief Defines the WebServer class, which processes a single Request at a time.
 */
#pragma once

#include "request.h"

/**
 * @class WebServer
 * @brief Simulates a single web server that processes one Request at a time.
 *
 * A WebServer is either idle or busy working through the time_remaining
 * of the Request it was last assigned. It does not manage a queue itself;
 * the LoadBalancer is responsible for assigning it new work.
 */
class WebServer {
    private:
        bool is_busy;
        Request curr_request;

    public:
        /// Constructs an idle WebServer.
        WebServer();

        /**
         * @brief Checks whether the server is currently processing a request.
         * @return true if busy, false if idle and available for new work.
         */
        bool busy() const;

        /**
         * @brief Assigns a new request to this server, marking it busy.
         * @param r The request to process.
         */
        void assign_request(Request r);

        /**
         * @brief Gets the remaining processing time of the current request.
         * @return Clock cycles remaining, or the last request's leftover value if idle.
         */
        int get_time_remaining();

        /**
         * @brief Advances the current request by one clock cycle.
         *
         * Decrements time_remaining; once it reaches zero or below, the
         * server becomes idle again. Does nothing if the server is idle.
         */
        void cycle();
};
