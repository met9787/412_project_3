/**
 * @file request.h
 * @brief Defines the Request struct representing a single web request.
 */
#pragma once

#include <string>

using std::string;

/**
 * @struct Request
 * @brief A single simulated web request handled by a WebServer.
 */
struct Request {
    string ip_in;         ///< Source IP address of the requester.
    string ip_out;        ///< Destination IP address of the result.
    int time_remaining;    ///< Clock cycles left before this request finishes processing.
    char job_type;         ///< 'p' for processing, 's' for streaming.
};
