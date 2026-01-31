#ifndef SESSION_H
#define SESSION_H

#include <string>
#include "database.h"
#include "helperfuncs.h"

// --- ANSI Color Codes ---
#define C_RESET       "\033[0m"
#define C_RED         "\033[1;31m"
#define C_GREEN       "\033[1;32m"
#define C_YELLOW      "\033[1;33m"
#define C_BLUE        "\033[1;34m"
#define C_MAGENTA     "\033[1;35m"
#define C_CYAN        "\033[1;36m"
#define C_BOLD        "\033[1m"

// The "Brain" of the application, holding all shared state and services.
struct Session {
    // Global Truths
    std::string interface;
    std::string my_ip;
    std::string my_mac;
    std::string target_ip;
    std::string target_mac;
    std::string gateway_ip;
    std::string gateway_mac;

    // Advanced Configurations
    std::string dhcp_server_ip; // Defaults to gateway_ip
    std::string dns_server_ip;  // Defaults to gateway_ip

    // Attack-specific states
    bool arp_spoof_active = false;

    // Shared Services
    Database* db;
    HelperFunctions* helper;

    Session() {
        db = new Database();
        helper = new HelperFunctions();
    }

    ~Session() {
        delete db;
        delete helper;
    }
};

#endif // SESSION_H
