/**
 * @file main.cpp
 * @brief Main driver program for the Load Balancer Simulation
 * @author [Your Name]
 * @date [Current Date]
 * 
 * This program simulates a load balancer managing web requests across multiple servers.
 * It demonstrates dynamic server scaling, request queue management, and performance monitoring.
 * 
 * Usage:
 * 1. Enter the number of servers (1-50)
 * 2. Enter the number of simulation cycles (100-50000)
 * 3. Watch the simulation run and observe load balancing behavior
 * 4. Review generated log files for analysis
 */

#include <iostream>
#include <limits>
#include <fstream>
#include "loadbalancer.h"
#include <iomanip> // Required for std::fixed and std::setprecision

/**
 * @brief Main function - Entry point of the load balancer simulation
 * @return 0 on successful execution
 * 
 * This function:
 * - Prompts user for simulation parameters
 * - Validates input values
 * - Creates and runs the load balancer simulation
 * - Generates log files with simulation results
 * - Displays summary statistics
 */
int main() {
    int num_servers;
    int total_cycles;

    std::cout << "===== Load Balancer Simulation =====\n";
    
    // Get number of servers with validation
    do {
        std::cout << "Enter number of servers (1-50): ";
        std::cin >> num_servers;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (num_servers < 1 || num_servers > 50) {
            std::cout << "Please enter a number between 1 and 50.\n";
            continue;
        }
        break;
    } while (true);

    // Get total clock cycles with validation
    do {
        std::cout << "Enter total clock cycles to run the simulation (100-50000): ";
        std::cin >> total_cycles;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        
        if (total_cycles < 100 || total_cycles > 50000) {
            std::cout << "Please enter a number between 100 and 50000.\n";
            continue;
        }
        break;
    } while (true);

    std::cout << "\nRunning simulation with " << num_servers << " servers for " 
              << total_cycles << " cycles...\n\n";

    // Open log file for detailed cycle-by-cycle data
    std::ofstream log_file("loadbalancer_log.csv");
    if (log_file.is_open()) {
        log_file << "Cycle,QueueSize,BusyServers,TotalServers,BlockedRequests,BlockedIPs\n";
    }

    // Use the same number for initial and max servers (allows scaling up to 2x the initial count)
    LoadBalancer lb(num_servers, num_servers * 2);
    
    // Run simulation with logging
    for (int i = 0; i < total_cycles; ++i) {
        lb.processCycle();
        
        // Write to log file every 100 cycles or at the end
        if (log_file.is_open() && (i % 100 == 0 || i == total_cycles - 1)) {
            lb.writeLogEntry(log_file);
        }
    }

    if (log_file.is_open()) {
        log_file.close();
        std::cout << "\nLog file saved as 'loadbalancer_log.csv'\n";
    }

    // Generate summary log file
    std::ofstream summary_log("log.txt");
    if (summary_log.is_open()) {
        summary_log << "===== Load Balancer Simulation Summary =====\n";
        summary_log << "Simulation Parameters:\n";
        summary_log << "- Number of servers: " << num_servers << "\n";
        summary_log << "- Maximum servers allowed: " << (num_servers * 2) << "\n";
        summary_log << "- Total cycles: " << total_cycles << "\n";
        summary_log << "- Request processing time range: 1-10 clock cycles\n\n";
        
        summary_log << "Simulation Results:\n";
        summary_log << "- Starting queue size: " << lb.getStartingQueueSize() << "\n";
        summary_log << "- Ending queue size: " << lb.getEndingQueueSize() << "\n";
        summary_log << "- Final total servers: " << lb.getTotalServers() << "\n";
        summary_log << "- Final busy servers: " << lb.getBusyServers() << "\n";
        summary_log << "- Queue change: " << (lb.getStartingQueueSize() - lb.getEndingQueueSize()) << " requests\n";
        summary_log << "- Blocked requests: " << lb.getBlockedRequests() << "\n";
        summary_log << "- Blocked IP addresses: " << lb.getBlockedIPCount() << "\n";
        
        summary_log << "\nPerformance Metrics:\n";
        summary_log << "- Average queue size: " << (lb.getStartingQueueSize() + lb.getEndingQueueSize()) / 2 << "\n";
        summary_log << "- Server utilization: " << std::fixed << std::setprecision(2) 
                   << (static_cast<double>(lb.getBusyServers()) / lb.getTotalServers() * 100) << "%\n";
        summary_log << "- Firewall effectiveness: " << std::fixed << std::setprecision(2)
                   << (static_cast<double>(lb.getBlockedRequests()) / (lb.getBlockedRequests() + lb.getEndingQueueSize() + lb.getStartingQueueSize()) * 100) << "% requests blocked\n";
        
        summary_log << "\nSimulation completed successfully!\n";
        summary_log.close();
        std::cout << "Summary log saved as 'log.txt'\n";
    }

    std::cout << "\n===== Simulation Summary =====\n";
    std::cout << "Starting queue size: " << lb.getStartingQueueSize() << "\n";
    std::cout << "Ending queue size: " << lb.getEndingQueueSize() << "\n";
    std::cout << "Blocked requests: " << lb.getBlockedRequests() << "\n";
    std::cout << "Blocked IP addresses: " << lb.getBlockedIPCount() << "\n";
    std::cout << "Range of task times: 1-10 clock cycles\n";

    return 0;
}
