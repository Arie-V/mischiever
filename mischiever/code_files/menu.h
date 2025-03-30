#ifndef MENU_H
#define MENU_H
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include "helperfuncs.h"
#include "protocols/syn.h"

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
    // All logos
    std::string logo = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌""";
    std::string logo2 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄""";
    std::string logo3 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄""";
    std::string logo4 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄ ▄\n""";
    std::string line = """▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n""";
};


#endif // MENU_H
