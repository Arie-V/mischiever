#include "menu.h"

// Constructor
Menu::Menu(){
    printLogo();
    printMenu();
}

// Destructor
Menu::~Menu(){}; 

void Menu::printLogo(){
    // cool logo transition
    this->helper.clearScreen();
    std::cout << this->logo << std::endl;
    sleep(1);
    this->helper.clearScreen();
    std::cout << this->logo2 << std::endl;
    sleep(1);
    this->helper.clearScreen();
    std::cout << this->logo3 << std::endl;
    sleep(1);
    this->helper.clearScreen();
    std::cout << this->logo4 << std::endl;
    std::cout << this->line << std::endl;
    sleep(1);
}

void Menu::printMenu(){
    // Menu options
    std::cout << "Welcome to Mischiever, ";
    while(true){
        std::cout << "Please select an option:" << std::endl;
        std::cout << "1. Select attack type" << std::endl;
        std::cout << "2. Funny cat picture" << std::endl;
        std::cout << "3. Exit\n" << std::endl;
        std::cout << "Your choice: ";

        // Get input
        int choice;
        std::cin >> choice;

        // Handle choice
        switch(choice){
            case 1:
                selectAttackType();
                break;
            case 2:
                funnyCatPicture();
                break;
            case 3:
                exit(0);
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
        
        std::cout << this->line << std::endl;
    }
}

void Menu::selectAttackType(){
    bool flag = true;
    while(flag){
        // Attack types
        std::cout << "\nSelect an attack type:" << std::endl;
        std::cout << "1. SYN Flood" << std::endl;
        std::cout << "2. ICMP Ping Flood" << std::endl;
        std::cout << "3. ARP Spoofing" << std::endl;
        std::cout << "4. Back to menu\n" << std::endl;
        std::cout << "Your choice: ";

        // Get input
        int choice;
        std::cin >> choice;

        // Handle choice
        switch(choice){
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                // Goes back to menu
                flag = false;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
        }
    }
}

void Menu::funnyCatPicture(){
    // Display funny cat picture
    this->helper.displayImage("misc/cat.jpg");
}