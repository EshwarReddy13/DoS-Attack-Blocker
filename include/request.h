#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief Struct to represent a web request in the load balancer system
 * 
 * This struct contains all the necessary information for a web request including
 * source and destination IP addresses, processing time requirements, and timing
 * information for tracking request lifecycle.
 */
struct Request {
    std::string ip_in;   ///< IP address of the requester (client)
    std::string ip_out;  ///< IP address of the target server (destination)
    int process_time;    ///< Number of clock cycles needed to process the request (1-10)
    int arrival_time;    ///< Clock cycle when the request entered the queue
    int assigned_time;   ///< Clock cycle when the request was assigned to a server (-1 if not assigned)
    bool processed;      ///< Whether the request has been completed

    /**
     * @brief Constructor for creating a new request
     * @param in Source IP address
     * @param out Destination IP address  
     * @param proc_time Processing time in clock cycles
     * @param arrival Clock cycle when request arrived
     */
    Request(std::string in, std::string out, int proc_time, int arrival);
};

#endif
