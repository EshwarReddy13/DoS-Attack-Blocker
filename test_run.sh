#!/bin/bash

echo "Testing Load Balancer..."
echo ""

# Create input file
echo "3" > test_input.txt
echo "10" >> test_input.txt
echo "100" >> test_input.txt

echo "Running with 3 servers, max 10, for 100 cycles..."
echo ""

# Run the program
./loadbalancer.exe < test_input.txt

echo ""
echo "Test complete!" 