# Load Balancer Simulation

## Project Overview

This project implements a C++ simulation of a load balancer that manages web requests across multiple servers. The simulation demonstrates dynamic server scaling, request queue management, and performance monitoring in a distributed computing environment.

## Prerequisites

### Required Software
- **C++ Compiler**: GCC 4.8+ or Clang 3.3+ or MSVC 2015+
  - **Windows**: MinGW-w64, Visual Studio, or WSL with GCC
  - **macOS**: Xcode Command Line Tools or Homebrew GCC
  - **Linux**: GCC (usually pre-installed)
- **Make**: For building with Makefile
  - **Windows**: MinGW Make, or use the provided batch files
  - **macOS/Linux**: Usually pre-installed

### Optional Software
- **Doxygen**: For generating documentation
  - **Windows**: Download from [doxygen.nl](https://www.doxygen.nl/download.html)
  - **macOS**: `brew install doxygen`
  - **Ubuntu/Debian**: `sudo apt-get install doxygen`
- **Git**: For version control (recommended)

### System Requirements
- **RAM**: 512MB minimum, 2GB recommended
- **Storage**: 100MB free space
- **OS**: Windows 10+, macOS 10.14+, or Linux (any modern distribution)

### Verifying Installation
To verify your setup, run these commands:

```bash
# Check C++ compiler
g++ --version

# Check Make
make --version

# Check Doxygen (optional)
doxygen --version
```

If any command fails, install the missing software using the instructions above.

## Features

- **Request Management**: Simulates web requests with random IP addresses and processing times
- **Server Pool**: Manages multiple web servers that can process requests concurrently
- **Dynamic Scaling**: Automatically adds/removes servers based on current load
- **Performance Monitoring**: Tracks queue size, server utilization, and processing metrics
- **Logging**: Generates detailed CSV logs and summary reports
- **Firewall Protection**: IP range blocking and DOS attack prevention

## Project Structure

```
Project 3/
├── main.cpp              # Main driver program
├── include/
│   ├── request.h         # Request struct definition
│   ├── webserver.h       # WebServer class definition
│   └── loadbalancer.h    # LoadBalancer class definition
├── src/
│   ├── request.cpp       # Request implementation
│   ├── webserver.cpp     # WebServer implementation
│   └── loadbalancer.cpp  # LoadBalancer implementation
├── Makefile              # Build configuration
├── Doxyfile              # Documentation configuration
└── README.md             # This file
```

## Building the Project

### Using Make (Unix/Linux)
```bash
make clean
make
./loadbalancer
```

### Using Make (Windows)
```bash
make clean
make
loadbalancer.exe
```

### Manual Compilation
```bash
g++ -std=c++11 -Iinclude -Wall -o loadbalancer main.cpp src/*.cpp
```

## Running the Simulation

1. **Enter number of servers** (1-50): The initial number of web servers
2. **Enter simulation cycles** (100-50000): How long to run the simulation
3. **Watch the output**: Real-time status updates every cycle
4. **Review logs**: Check generated log files for analysis

## Output Files

- **loadbalancer_log.csv**: Detailed cycle-by-cycle data (every 100 cycles)
- **log.txt**: Summary report with performance metrics

## Documentation

### Generating Documentation

This project uses Doxygen to generate HTML documentation.

#### Prerequisites
- Install Doxygen: `sudo apt-get install doxygen` (Ubuntu/Debian)
- Or download from: https://www.doxygen.nl/download.html

#### Generate Documentation
```bash
make docs
```

#### View Documentation
Open `docs/html/index.html` in your web browser.

### Documentation Structure

The generated documentation includes:
- **Class Documentation**: Detailed descriptions of all classes and methods
- **File Documentation**: Source code documentation with inline comments
- **Class Diagrams**: Visual representation of class relationships
- **Function Documentation**: Parameter descriptions and return values

## Class Overview

### Request Struct
Represents a web request with:
- Source and destination IP addresses
- Processing time requirements
- Timing information for tracking

### WebServer Class
Simulates a web server that:
- Processes one request at a time
- Tracks processing time
- Reports completion status

### LoadBalancer Class
Manages the overall system:
- Distributes requests across servers
- Scales server count dynamically
- Monitors performance metrics
- Generates simulation data

## Simulation Parameters

- **Request Processing Time**: 1-10 clock cycles (random)
- **New Request Rate**: 10% chance per cycle
- **Scaling Threshold**: Add servers when queue > 2x server count
- **Initial Queue**: Pre-filled with (servers × 100) requests

## Firewall Features

The load balancer includes advanced firewall protection:

### IP Range Blocking
- **Private Networks**: Automatically blocks 192.168.x.x, 10.x.x.x, and 127.x.x.x ranges
- **Rate Limiting**: Blocks IPs that make more than 50 requests
- **Dynamic Blocking**: Maintains a blacklist of suspicious IP addresses

### DOS Attack Prevention
- **Request Tracking**: Monitors request frequency per IP address
- **Automatic Blocking**: Blocks IPs exhibiting suspicious behavior
- **Real-time Logging**: Logs all blocked requests and IP addresses

### Firewall Statistics
- **Blocked Requests**: Total number of requests blocked
- **Blocked IPs**: Number of unique IP addresses blocked
- **Effectiveness**: Percentage of requests blocked by firewall

## Performance Analysis

The simulation provides several metrics:
- **Queue Size**: Number of pending requests
- **Server Utilization**: Percentage of busy servers
- **Scaling Events**: When servers are added/removed
- **Processing Efficiency**: Request completion rates

## Requirements Compliance

This project satisfies all CSCE 412 Project 3 requirements:
- ✅ C++ implementation with proper class design
- ✅ Queue data structure for request management
- ✅ Dynamic server allocation and deallocation
- ✅ Random request generation and timing
- ✅ Comprehensive logging and documentation
- ✅ Doxygen HTML documentation generation

## Author

[Your Name] - CSCE 412 Project 3

## License

This project is created for educational purposes as part of CSCE 412 coursework. 