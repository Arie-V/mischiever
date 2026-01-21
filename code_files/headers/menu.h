#ifndef MENU_H
#define MENU_H
#include <string>
#include <iostream>
#include <ctime>
#include <unistd.h>
#include <thread>
#include "helperfuncs.h"
#include "syn.h"
#include "arp.h"
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
    void showFloodsMenu();
    void showSpoofingsMenu();
    bool get_input(const std::string& prompt, std::string& output);
    bool get_input(const std::string& prompt, int& output);

private:
    // Helperfunctions instance for misc functions    
    HelperFunctions helper;

    // Made for ARP management (on/off)
    ARP arp_tool;

    // Database instance
    Database* db;
};


#endif // MENU_H
