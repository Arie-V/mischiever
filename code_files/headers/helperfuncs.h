#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <ifaddrs.h>
#include <limits>
#include <net/if.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <iomanip>
#include <unistd.h>
#include <arpa/inet.h>

// Define colors
#define C_RESET       "\033[0m"
#define C_GREEN       "\033[1;32m"
#define C_YELLOW      "\033[1;33m"
#define C_BLUE        "\033[1;34m"
#define C_BOLD        "\033[1m"

class HelperFunctions {
    public:
    void clearScreen();
    void displayImage(const char* filename);
    const char* get_iface();
    std::string get_local_ip(const char* iface);
    
    // Resolves an IP (e.g., "192.168.1.5") to a MAC string (e.g., "00:0c:...")
    std::string get_mac_from_ip(std::string ip_addr);

    // Input validation
    bool is_valid_ip(const std::string& ip);
    bool is_valid_mac(const std::string& mac);

    // Scanner function to find all active devices on the local network
    void scan_local_network(const char* interface);

    // Scanner function to find the default gateway's IP
    std::string get_default_gateway_ip();
};

#endif
