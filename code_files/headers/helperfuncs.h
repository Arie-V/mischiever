#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <ifaddrs.h>
#include <net/if.h>
#include <iostream>
#include <fstream>
#include <sstream>

class HelperFunctions {
    public:
    void clearScreen();
    void displayImage(const char* filename);
    const char* get_iface();
    
    // Resolves an IP (e.g., "192.168.1.5") to a MAC string (e.g., "00:0c:...")
    std::string get_mac_from_ip(std::string ip_addr);
};

#endif
