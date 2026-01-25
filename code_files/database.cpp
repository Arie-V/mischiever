#include "headers/database.h"
#include <ctime>

// ANSI Colors
#define C_RESET       "\033[0m"
#define C_RED         "\033[1;31m"
#define C_GREEN       "\033[1;32m"
#define C_YELLOW      "\033[1;33m"
#define C_BLUE        "\033[1;34m"
#define C_MAGENTA     "\033[1;35m"
#define C_CYAN        "\033[1;36m"
#define C_BOLD        "\033[1m"

// Callback function used by sqlite3_exec to print results
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << C_CYAN << azColName[i] << ": " << C_RESET 
                  << C_YELLOW << (argv[i] ? argv[i] : "NULL") << C_RESET;
        if (i < argc - 1) std::cout << C_MAGENTA << " | " << C_RESET;
    }
    std::cout << std::endl;
    return 0;
}

Database::Database() {
    int rc = sqlite3_open("mischiever_history.db", &db);
    
    if (rc) {
        std::cerr << "[-] Can't open database: " << sqlite3_errmsg(db) << std::endl;
    } else {
        // Create Table if not exists
        const char* sql = "CREATE TABLE IF NOT EXISTS ATTACKS(" \
                          "ID INTEGER PRIMARY KEY AUTOINCREMENT," \
                          "TYPE           TEXT    NOT NULL," \
                          "DATE           TEXT    DEFAULT (DATE('now'))," \
                          "TIME           TEXT    DEFAULT (TIME('now'))," \
                          "ATTACKER_IP    TEXT    NOT NULL," \
                          "VICTIM_IP      TEXT    NOT NULL);";
        
        char* zErrMsg = 0;
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        
        if (rc != SQLITE_OK) {
            std::cerr << "SQL error: " << zErrMsg << std::endl;
            sqlite3_free(zErrMsg);
        }
    }
}

Database::~Database() {
    sqlite3_close(db);
}

void Database::log_attack(std::string type, std::string attacker_ip, std::string victim_ip) {
    std::string sql = "INSERT INTO ATTACKS (TYPE, ATTACKER_IP, VICTIM_IP) " \
                      "VALUES ('" + type + "', '" + attacker_ip + "', '" + victim_ip + "');";

    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL insert error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
}

void Database::print_history() {
    const char* sql = "SELECT * from ATTACKS";
    char* zErrMsg = 0;
    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << C_RED << "SQL error: " << zErrMsg << C_RESET << std::endl;
        sqlite3_free(zErrMsg);
    }
}