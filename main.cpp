#include <iostream>
#include <string>
#include "code_files/headers/menu.h"
#include "code_files/headers/database.h"

/*
Compilation command:

g++ -c main.cpp
g++ -c code_files/helperfuncs.cpp
g++ -c code_files/menu.cpp
g++ -c code_files/processes.cpp
g++ -c code_files/protocols/syn.cpp
g++ -c code_files/protocols/arp.cpp
g++ -o mischiever main.o helperfuncs.o menu.o syn.o arp.o processes.o
rm *.o
*/

int main() {
    // 1. Initialize Database
    Database db;

    // 2. Pass DB to Menu and initialize it
    Menu menu(&db); 

    return 0;
}