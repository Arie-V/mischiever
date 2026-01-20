#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>
#include <string>
#include <iostream>

class Database {
private:
    sqlite3* db;
    
public:
    // Constructor opens DB and creates table
    Database();
    
    // Destructor closes DB
    ~Database();

    // Logs a new attack
    void log_attack(std::string type, std::string attacker_ip, std::string victim_ip);

    // Prints all history
    void print_history();
};

#endif