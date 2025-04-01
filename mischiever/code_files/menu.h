#ifndef MENU_H
#define MENU_H
#define WAITING_TIME 1
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <thread>
#include "helperfuncs.h"
#include "protocols/syn.h"
#include "protocols/arp.h"
#include "processes.h"

class Menu {
public:
    // Constructor
    Menu();

    // Destructor
    ~Menu();

    void printLogo();
    void printMenu();

    void selectAttackType();
    void funnyCatPicture();
    
private:
    // Helperfunctions instance for misc functions    
    HelperFunctions helper;

    // Procceses instance for managing threads
    Processes processes;

    // All logos
    std::string logo = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌""";
    std::string logo2 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄""";
    std::string logo3 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄""";
    std::string logo4 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄ ▄\n""";
    std::string line = """▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n""";
};


#endif // MENU_H
