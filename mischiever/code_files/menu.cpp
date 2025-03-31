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
    sleep(WAITING_TIME);
    this->helper.clearScreen();
    std::cout << this->logo2 << std::endl;
    sleep(WAITING_TIME);
    this->helper.clearScreen();
    std::cout << this->logo3 << std::endl;
    sleep(WAITING_TIME);
    this->helper.clearScreen();
    std::cout << this->logo4 << std::endl;
    std::cout << this->line << std::endl;
    sleep(WAITING_TIME);
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
        std::cout << "1. SYN Flood (different source ip's each packet)" << std::endl;
        std::cout << "2. ARP Spoofing" << std::endl;
        std::cout << "3. ICMP Ping Flood" << std::endl;
        std::cout << "4. Back to menu\n" << std::endl;
        std::cout << "Your choice: ";

        // Get input
        int choice;
        std::cin >> choice;

        // Handle choice
        switch(choice){
            case 1: {
                std::cout << "Input the target IP: ";
                std::string target_ip_str;
                std::cin >> target_ip_str;
                const char* target_ip = target_ip_str.c_str(); // Convert to const char*

                std::cout << "Input the target port: ";
                int target_port;
                std::cin >> target_port;

                std::cout << "Input the number of packets: ";
                int packet_count;
                std::cin >> packet_count;

                // SYN flood
                SYN syn_instance;
                syn_instance.syn_flood(target_ip, target_port, packet_count);

                break;
            }
            case 2: {
                const char* iface;
                std::cout << "Getting current device's interface..." << std::endl;
                sleep(WAITING_TIME);
                iface = helper.get_iface();
                std::cout << "Your current interface is: " << iface << std::endl;

                std::cout << "Input the target IP (Victim's IP): ";
                std::string target_ip_str;
                std::cin >> target_ip_str;
                const char* target_ip = target_ip_str.c_str(); // Convert to const char*

                std::cout << "Input the spoofed IP (Victim will think this is your IP): ";
                std::string spoof_ip_str;
                std::cin >> spoof_ip_str;
                const char* spoof_ip = spoof_ip_str.c_str(); // Convert to const char*

                // ARP spoofing
                ARP arp_instance;
                arp_instance.send_arp_spoof(iface, target_ip, spoof_ip);

                break;
            }
            case 3: {
                break;
            }
            case 4: {
                // Goes back to menu
                flag = false;
                break;
            }
            default: {
                std::cout << "Invalid choice. Please try again." << std::endl;
                break;
            }
        }
    }
}

void Menu::funnyCatPicture(){
    // Display funny cat picture
    this->helper.displayImage("misc/cat.jpg");
}