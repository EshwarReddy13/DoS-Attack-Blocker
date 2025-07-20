#include "loadbalancer.h"
#include <iostream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <fstream> // Added for writeLogEntry

LoadBalancer::LoadBalancer(int initial_servers, int max_serv)
    : current_time(0), min_servers(initial_servers), max_servers(max_serv), server_id_counter(0),
      blocked_requests(0), max_requests_per_ip(50) // Allow 50 requests per IP before blocking
{
    // Initialize random seed
    srand(static_cast<unsigned int>(time(nullptr)));
    
    std::cout << "Initializing " << min_servers << " servers..." << std::endl;
    
    for (int i = 0; i < min_servers; ++i) {
        servers.push_back(new WebServer());
    }
    
    std::cout << "Pre-filling queue with " << (min_servers * 100) << " requests..." << std::endl;
    
    // Pre-fill queue
    for (int i = 0; i < min_servers * 100; ++i) {
        Request* req = generateRandomRequest(current_time);
        if (req) {
            request_queue.push(req);
        }
    }
    
    std::cout << "Load balancer initialization complete." << std::endl;
}

LoadBalancer::~LoadBalancer() {
    // Clean up servers and any active requests
    for (auto* s : servers) {
        if (s->isBusy()) {
            Request* req = s->finishRequest();
            if (req) delete req;
        }
        delete s;
    }
    
    // Clean up remaining requests in queue
    while (!request_queue.empty()) {
        delete request_queue.front();
        request_queue.pop();
    }
}

void LoadBalancer::processCycle() {
    current_time++;
    addRequest();
    assignRequests();

    // Process each server and handle completed requests
    for (auto* s : servers) {
        s->processCycle();
        
        // Check if server finished a request
        if (s->isRequestDone()) {
            Request* completed = s->finishRequest();
            if (completed) {
                // Log completed request (optional)
                // std::cout << "Request completed: " << completed->ip_in << " -> " << completed->ip_out << std::endl;
                delete completed; // Clean up completed request
            }
        }
    }

    scaleServers();
    printStatus();
}

void LoadBalancer::addRequest() {
    // 10% chance to add a new request each tick
    if (rand() % 10 == 0) {
        Request* new_request = generateRandomRequest(current_time);
        if (new_request) {
            request_queue.push(new_request);
        }
        // If generateRandomRequest returns nullptr, the request was blocked
    }
}

void LoadBalancer::assignRequests() {
    for (auto* s : servers) {
        if (!s->isBusy() && !request_queue.empty()) {
            Request* req = request_queue.front();
            request_queue.pop();
            s->assignRequest(req, current_time);
        }
    }
}

void LoadBalancer::scaleServers() {
    int load = request_queue.size();
    int active = servers.size();
    int busy_servers = 0;
    
    // Count busy servers
    for (auto* s : servers) {
        if (s->isBusy()) busy_servers++;
    }

    // If overloaded, add a server (up to max)
    if (load > active * 2 && active < max_servers) {
        servers.push_back(new WebServer());
        std::cout << "  [SCALE UP] Added server. Total: " << servers.size() << std::endl;
    }
    // If underloaded and more than min servers, remove one (only if no busy servers)
    // Use a more conservative threshold: scale down only when queue is very small
    else if (load <= 5 && active > min_servers && busy_servers == 0) {
        delete servers.back();
        servers.pop_back();
        std::cout << "  [SCALE DOWN] Removed server. Total: " << servers.size() << std::endl;
    }
}

void LoadBalancer::printStatus() {
    int active = 0;
    for (auto* s : servers) if (s->isBusy()) active++;

    std::cout << "[Cycle " << std::setw(5) << current_time << "] "
              << "Queue: " << std::setw(4) << request_queue.size()
              << " | Active Servers: " << std::setw(2) << active
              << " | Total Servers: " << std::setw(2) << servers.size();
    
    // Add firewall status if there are blocked requests
    if (blocked_requests > 0) {
        std::cout << " | Blocked: " << std::setw(3) << blocked_requests 
                  << " (" << blocked_ips.size() << " IPs)";
    }
    
    std::cout << std::endl;
}

Request* LoadBalancer::generateRandomRequest(int arrival_time) {
    std::string in = generateRandomIP();
    std::string out = generateRandomIP();
    
    // Debug: Check if IPs are valid
    if (in.empty() || out.empty()) {
        std::cerr << "Error: Generated empty IP address" << std::endl;
        return nullptr;
    }
    
    // Check if source IP should be blocked
    if (isIPBlocked(in)) {
        blocked_requests++;
        return nullptr; // Block the request
    }
    
    // Track request count for this IP (safer access)
    if (ip_request_count.find(in) == ip_request_count.end()) {
        ip_request_count[in] = 1;
    } else {
        ip_request_count[in]++;
    }
    
    // Check if this IP has made too many requests
    if (ip_request_count[in] > max_requests_per_ip) {
        blockIP(in);
        blocked_requests++;
        return nullptr; // Block the request
    }
    
    int proc_time = 1 + rand() % 10; // Range: 1-10 clock cycles
    return new Request(in, out, proc_time, arrival_time);
}

std::string LoadBalancer::generateRandomIP() {
    try {
        std::string ip = std::to_string(rand() % 256) + "." +
                        std::to_string(rand() % 256) + "." +
                        std::to_string(rand() % 256) + "." +
                        std::to_string(rand() % 256);
        
        // Validate the generated IP
        if (ip.length() < 7 || ip.length() > 15) {
            return "0.0.0.0"; // Return a safe default
        }
        
        return ip;
    } catch (...) {
        return "0.0.0.0"; // Return a safe default on any error
    }
}

bool LoadBalancer::isIPBlocked(const std::string& ip) {
    // Check if IP is empty or too short
    if (ip.empty() || ip.length() < 7) {
        return false;
    }
    
    // Check if IP is in blocked set
    if (blocked_ips.find(ip) != blocked_ips.end()) {
        return true;
    }
    
    // Block certain IP ranges (simulating firewall rules)
    // Block 192.168.x.x (private network)
    if (ip.length() >= 8 && ip.substr(0, 8) == "192.168.") {
        return true;
    }
    
    // Block 10.x.x.x (private network)
    if (ip.length() >= 3 && ip.substr(0, 3) == "10.") {
        return true;
    }
    
    // Block 127.x.x.x (localhost)
    if (ip.length() >= 4 && ip.substr(0, 4) == "127.") {
        return true;
    }
    
    return false;
}

void LoadBalancer::blockIP(const std::string& ip) {
    blocked_ips.insert(ip);
    std::cout << "  [FIREWALL] Blocked IP: " << ip << " (too many requests)" << std::endl;
}

int LoadBalancer::getStartingQueueSize() const {
    return min_servers * 100;
}

int LoadBalancer::getEndingQueueSize() const {
    return request_queue.size();
}

int LoadBalancer::getBusyServers() const {
    int busy_count = 0;
    for (auto* s : servers) {
        if (s->isBusy()) busy_count++;
    }
    return busy_count;
}

void LoadBalancer::writeLogEntry(std::ofstream& log_file) const {
    log_file << current_time << ","
             << request_queue.size() << ","
             << getBusyServers() << ","
             << servers.size() << ","
             << blocked_requests << ","
             << blocked_ips.size() << "\n";
}
