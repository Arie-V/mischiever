#ifndef MENU_H
#define MENU_H
#define WAITING_TIME 1
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <thread>
#include "helperfuncs.h"
#include "syn.h"
#include "arp.h"
#include "processes.h"
#include "database.h"

class Menu {
public:
    // Constructor
    Menu(Database* database);

    // Destructor
    ~Menu();

    void printLogo();
    void printMenu();

    void selectAttackType();
    void funnyCatPicture();
    
private:
    // Helperfunctions instance for misc functions    
    HelperFunctions helper;

    // Made for ARP management (on/off)
    ARP arp_tool;

    // Database instance
    Database* db;

    // Procceses instance for managing threads
    Processes processes;

    // All logos
    std::string logo = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌""";
    std::string logo2 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄""";
    std::string logo3 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄""";
    std::string line = """▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n""";
};


#endif // MENU_H
