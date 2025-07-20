#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "request.h"

/**
 * @brief Class to represent a web server that handles requests
 * 
 * This class simulates a web server that can process one request at a time.
 * It tracks the current request being processed, remaining processing time,
 * and provides methods to assign new requests and check completion status.
 */
class WebServer {
private:
    bool busy;              ///< Whether the server is currently processing a request
    int time_remaining;     ///< Number of clock cycles remaining to complete current request
    Request* current_request; ///< Pointer to the request currently being processed

public:
    /**
     * @brief Default constructor
     * 
     * Initializes a new web server in idle state with no current request.
     */
    WebServer();
    
    /**
     * @brief Check if the server is currently busy
     * @return true if server is processing a request, false otherwise
     */
    bool isBusy() const;
    
    /**
     * @brief Assign a new request to this server
     * @param req Pointer to the request to be processed
     * @param current_time Current clock cycle for timing tracking
     * 
     * Only assigns the request if the server is not currently busy.
     */
    void assignRequest(Request* req, int current_time);
    
    /**
     * @brief Process one clock cycle
     * 
     * Decrements the remaining processing time for the current request.
     * If processing time reaches zero, marks the request as completed.
     */
    void processCycle();
    
    /**
     * @brief Check if the current request is finished
     * @return true if request is completed, false otherwise
     */
    bool isRequestDone() const;
    
    /**
     * @brief Finish and return the completed request
     * @return Pointer to the completed request, or nullptr if no request was completed
     * 
     * This method should be called after isRequestDone() returns true.
     * It returns the request pointer and resets the server to idle state.
     */
    Request* finishRequest();
};

#endif
