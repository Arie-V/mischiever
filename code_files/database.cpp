#include "headers/database.h"
#include <ctime>

// Callback function used by sqlite3_exec to print results
static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << " | ";
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
    
    std::cout << "\n=== ATTACK HISTORY LOG ===" << std::endl;
    int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    std::cout << "==========================\n" << std::endl;
}