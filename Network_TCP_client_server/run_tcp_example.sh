#!/bin/bash
#
# Script Name: Kill_application.sh
# Description: Brief description of the script's purpose.
# Author: Albert Domingo
# GitHub: https://github.com/AlbertDM
#
# Additional Information:
# - Add any additional information or notes about the script here.
# - You can provide details on usage, dependencies, or any other relevant information.
#
# Usage:
# - Provide instructions on how to use the script, including command-line arguments, if any.
#
# Example:
# - Show an example of how to run the script, demonstrating its usage.
#

# Start writing your script here...

#!/bin/bash

# TCP server variables
SERVER_BINARY="./tcp_server"
SERVER_PID=""

# TCP client variables
CLIENT_BINARY="./tcp_client"
CLIENT_PID=""

# Function to start the TCP server
start_server() {
    $SERVER_BINARY &
    SERVER_PID=$!
    echo "TCP server started with PID: $SERVER_PID"
}

# Function to stop the TCP server
stop_server() {
    if [[ -n $SERVER_PID ]]; then
        echo "Stopping TCP server..."
        kill $SERVER_PID
        wait $SERVER_PID 2>/dev/null
        SERVER_PID=""
        echo "TCP server stopped."
    else
        echo "TCP server is not running."
    fi
}

# Function to start the TCP client
start_client() {
    $CLIENT_BINARY &
    CLIENT_PID=$!
    echo "TCP client started with PID: $CLIENT_PID"
}

# Function to stop the TCP client
stop_client() {
    if [[ -n $CLIENT_PID ]]; then
        echo "Stopping TCP client..."
        kill $CLIENT_PID
        wait $CLIENT_PID 2>/dev/null
        CLIENT_PID=""
        echo "TCP client stopped."
    else
        echo "TCP client is not running."
    fi
}

# Function to gracefully stop both server and client
stop_all() {
    stop_client
    stop_server
}

# Main script logic

echo "TCP Server-Client Script"

while true; do
    echo ""
    echo "1. Start TCP server"
    echo "2. Stop TCP server"
    echo "3. Start TCP client"
    echo "4. Stop TCP client"
    echo "5. Stop both server and client"
    echo "6. Exit"

    read -p "Enter your choice (1-6): " choice

    case $choice in
        1) start_server;;
        2) stop_server;;
        3) start_client;;
        4) stop_client;;
        5) stop_all;;
        6) break;;
        *) echo "Invalid choice";;
    esac
done

echo "Exiting..."



