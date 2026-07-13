/**
 * @file load_balancer.h
 * @brief Defines the LoadBalancer class, which can act as a router or a worker.
 */
#pragma once

#include "request.h"
#include "web_server.h"

#include <ostream>
#include <queue>
#include <string>
#include <vector>

using std::queue, std::vector, std::string, std::ostream;

/**
 * @enum LBMode
 * @brief The role a given LoadBalancer instance plays in the simulation.
 */
enum class LBMode {
    ROUTER,      ///< Generates/receives traffic and dispatches by job_type; owns no servers.
    PROCESSING,  ///< Owns WebServers and handles 'p' (processing) requests.
    STREAMING    ///< Owns WebServers and handles 's' (streaming) requests.
};

/**
 * @class LoadBalancer
 * @brief Manages either a pool of WebServers (worker mode) or the dispatch of
 *        requests to other LoadBalancer instances (router mode).
 *
 * A single class supports both roles via LBMode, per the assignment's note
 * that routing between load balancers "can be a setting in your load
 * balancer instance." Worker instances (PROCESSING/STREAMING) dynamically
 * grow/shrink their server pool based on queue thresholds so it neither
 * sits empty nor grows unbounded.
 */
class LoadBalancer {
    private:
        LBMode mode;
        int current_time;
        int upper_threshold;
        int lower_threshold;
        int min_servers;
        int max_servers;
        queue<Request> request_q;
        vector<WebServer> servers;

        // Only used when mode == ROUTER.
        LoadBalancer* processing_lb;
        LoadBalancer* streaming_lb;

        /// Grows or shrinks the server pool based on current queue size. Worker mode only.
        void scale();

    public:
        /**
         * @brief Constructs a worker LoadBalancer (PROCESSING or STREAMING).
         * @param mode Must be LBMode::PROCESSING or LBMode::STREAMING.
         * @param num_servers Initial number of WebServers to create.
         */
        LoadBalancer(LBMode mode, int num_servers);

        /**
         * @brief Constructs a router LoadBalancer that dispatches to two worker LoadBalancers.
         * @param processing Worker LoadBalancer that should receive 'p' job_type requests.
         * @param streaming Worker LoadBalancer that should receive 's' job_type requests.
         * @param initial_requests Number of requests to pre-generate into the router's queue.
         */
        LoadBalancer(LoadBalancer* processing, LoadBalancer* streaming, int initial_requests);

        /**
         * @brief Advances the simulation by one clock cycle.
         *
         * Router mode: forwards every queued request to the appropriate
         * worker LoadBalancer based on job_type.
         * Worker mode: assigns queued requests to idle servers, ticks busy
         * servers, then rebalances the server pool via scale().
         */
        void cycle();

        /**
         * @brief Generates a single random Request (random IPs, time, and job_type).
         * @return The newly generated Request.
         */
        Request create_request();

        /**
         * @brief Enqueues a request for this LoadBalancer to handle.
         * @param r The request to add.
         */
        void add_request(Request r);

        /// @return Clock cycles elapsed for this instance.
        int get_time() const;
        /// @return Number of requests currently queued.
        int get_queue_size() const;
        /// @return Number of WebServers currently owned (0 for router mode).
        int get_num_servers() const;
        /// @return Number of currently busy WebServers (0 for router mode).
        int get_busy_servers() const;

        /**
         * @brief Writes a single status line for this cycle to a log stream.
         * @param out Output stream (e.g. an open log file).
         * @param label Human-readable name for this instance (e.g. "processing").
         */
        void log_status(ostream& out, const string& label) const;
};
