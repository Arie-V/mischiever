#ifndef SESSION_H
#define SESSION_H

#include <string>
#include "database.h"
#include "helperfuncs.h"

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
