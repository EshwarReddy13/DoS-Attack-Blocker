#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include "request.h"
#include "webserver.h"

/**
 * @brief Class that manages web servers and a queue of requests to simulate load balancing
 * 
 * This class implements a load balancer that distributes incoming web requests across
 * multiple web servers. It includes features for dynamic server scaling, request queue
 * management, and performance monitoring. The load balancer can automatically add or
 * remove servers based on current load to maintain optimal performance.
 * 
 * Additional features include IP range blocking for firewall/DOS attack prevention.
 */
class LoadBalancer {
private:
    std::queue<Request*> request_queue;         ///< Queue holding incoming requests waiting to be processed
    std::vector<WebServer*> servers;            ///< Pool of dynamically managed web servers
    int current_time;                           ///< Current simulation clock cycle
    int min_servers;                            ///< Minimum number of servers to maintain
    int max_servers;                            ///< Maximum number of servers allowed for scaling
    int server_id_counter;                      ///< Counter for assigning unique server IDs
    
    // IP blocking functionality
    std::map<std::string, int> ip_request_count; ///< Track request count per IP
    std::set<std::string> blocked_ips;          ///< Set of blocked IP addresses
    int blocked_requests;                       ///< Total number of blocked requests
    int max_requests_per_ip;                    ///< Maximum requests allowed per IP before blocking

    /**
     * @brief Generate a random request for simulation
     * @param arrival_time Current clock cycle when request is generated
     * @return Pointer to a new Request object, or nullptr if request is blocked
     */
    Request* generateRandomRequest(int arrival_time);
    
    /**
     * @brief Generate a random IP address string
     * @return String representation of a random IP address (x.x.x.x format)
     */
    std::string generateRandomIP();
    
    /**
     * @brief Check if an IP address should be blocked
     * @param ip IP address to check
     * @return true if IP should be blocked, false otherwise
     */
    bool isIPBlocked(const std::string& ip);
    
    /**
     * @brief Block an IP address due to suspicious activity
     * @param ip IP address to block
     */
    void blockIP(const std::string& ip);

public:
    /**
     * @brief Constructor for creating a new load balancer
     * @param initial_servers Number of servers to start with
     * @param max_serv Maximum number of servers allowed (default: 100)
     * 
     * Initializes the load balancer with the specified number of servers and
     * pre-fills the request queue with initial requests for simulation.
     */
    LoadBalancer(int initial_servers, int max_serv = 100);
    
    /**
     * @brief Destructor to clean up allocated memory
     * 
     * Properly deallocates all servers and any remaining requests in the queue.
     */
    ~LoadBalancer();

    /**
     * @brief Process one simulation cycle
     * 
     * This is the main simulation method that:
     * - Increments the clock
     * - Adds new random requests
     * - Assigns requests to available servers
     * - Processes all servers
     * - Scales servers based on load
     * - Prints current status
     */
    void processCycle();
    
    /**
     * @brief Add a new random request to the queue
     * 
     * Has a 10% chance each cycle to add a new request to simulate
     * real-world incoming traffic patterns.
     */
    void addRequest();
    
    /**
     * @brief Assign queued requests to available servers
     * 
     * Iterates through all servers and assigns requests from the queue
     * to any servers that are not currently busy.
     */
    void assignRequests();
    
    /**
     * @brief Scale servers up or down based on current load
     * 
     * Automatically adds servers when queue size > 2x server count,
     * and removes servers when queue is small and no servers are busy.
     */
    void scaleServers();
    
    /**
     * @brief Print current simulation status
     * 
     * Displays current cycle, queue size, active servers, and total servers.
     */
    void printStatus();

    /**
     * @brief Get the initial queue size
     * @return Number of requests that were in the queue at simulation start
     */
    int getStartingQueueSize() const;
    
    /**
     * @brief Get the current queue size
     * @return Number of requests currently waiting in the queue
     */
    int getEndingQueueSize() const;
    
    /**
     * @brief Get the current simulation time
     * @return Current clock cycle number
     */
    int getCurrentTime() const { return current_time; }
    
    /**
     * @brief Get the total number of servers
     * @return Current number of servers (including idle and busy)
     */
    int getTotalServers() const { return servers.size(); }
    
    /**
     * @brief Get the number of busy servers
     * @return Number of servers currently processing requests
     */
    int getBusyServers() const;
    
    /**
     * @brief Get the current queue size
     * @return Number of requests in the queue
     */
    int getQueueSize() const { return request_queue.size(); }
    
    /**
     * @brief Write a log entry to the specified file
     * @param log_file Reference to the output file stream
     * 
     * Writes a CSV line with current time, queue size, busy servers, and total servers.
     */
    void writeLogEntry(std::ofstream& log_file) const;
    
    /**
     * @brief Get the number of blocked requests
     * @return Total number of requests blocked by firewall
     */
    int getBlockedRequests() const { return blocked_requests; }
    
    /**
     * @brief Get the number of blocked IP addresses
     * @return Number of unique IP addresses currently blocked
     */
    int getBlockedIPCount() const { return blocked_ips.size(); }
};

#endif
