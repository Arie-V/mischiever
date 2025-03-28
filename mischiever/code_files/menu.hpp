#ifndef MENU_HPP
#define MENU_HPP
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include "helperfuncs.hpp"

class Menu {
public:
    // Constructor
    Menu(){
        printLogo();
    }

    // Destructor
    ~Menu(){}

    void printLogo(){
        // cool logo transition
        helper.clearScreen();
        std::cout << logo << std::endl;
        sleep(1);
        helper.clearScreen();
        std::cout << logo2 << std::endl;
        sleep(1);
        helper.clearScreen();
        std::cout << logo3 << std::endl;
        sleep(1);
        helper.clearScreen();
        std::cout << logo4 << std::endl;
        sleep(1);
        sleep(1);
    }
    void printMenu(){
        // Menu options
        std::cout << "Welcome to Mischiever, ";
        while(true){
            std::cout << "Please select an option:" << std::endl;
            std::cout << "1. Select attack type" << std::endl;
            std::cout << "2. Funny cat picture" << std::endl;
            std::cout << "3. Exit\n" << std::endl;
        }
    }

private:
    // Hlperfunctions instance for misc functions    
    HelperFunctions helper;
    // All logos
    std::string logo = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌""";
    std::string logo2 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄""";
    std::string logo3 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄""";
    std::string logo4 = """\n▗▖  ▗▖▗▄▄▄▖ ▗▄▄▖ ▗▄▄▖▗▖ ▗▖▗▄▄▄▖▗▄▄▄▖▗▖  ▗▖▗▄▄▄▖▗▄▄▖ \n▐▛▚▞▜▌  █  ▐▌   ▐▌   ▐▌ ▐▌  █  ▐▌   ▐▌  ▐▌▐▌   ▐▌ ▐▌\n▐▌  ▐▌  █   ▝▀▚▖▐▌   ▐▛▀▜▌  █  ▐▛▀▀▘▐▌  ▐▌▐▛▀▀▘▐▛▀▚▖\n▐▌  ▐▌▗▄█▄▖▗▄▄▞▘▝▚▄▄▖▐▌ ▐▌▗▄█▄▖▐▙▄▄▖ ▝▚▞▘ ▐▙▄▄▖▐▌ ▐▌ ▄ ▄ ▄\n▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄\n""";
};


#endif // MENU_HPP
