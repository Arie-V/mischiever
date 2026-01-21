#include <iostream>
#include <string>
#include "code_files/headers/menu.h"
#include "code_files/headers/database.h"

int main() {
    // 1. Initialize Database
    Database db;

    // 2. Pass DB to Menu and initialize it
    Menu menu(&db); 

    return 0;
}