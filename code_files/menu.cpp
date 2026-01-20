#include "headers/menu.h"

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
    // cool logo transition
    this->helper.clearScreen();
    std::cout << this->logo << std::endl;
    sleep(WAITING_TIME);
    this->helper.clearScreen();
    std::cout << this->logo2 << std::endl;
    sleep(WAITING_TIME);
    this->helper.clearScreen();
    std::cout << this->logo3 << std::endl;
    std::cout << this->line << std::endl;
}

void Menu::printMenu(){
    // Menu options
    std::cout << "Welcome to Mischiever, ";
    while(true){
        std::cout << "Please select an option:" << std::endl;
        std::cout << "1. Select attack type" << std::endl;
        std::cout << "2. Attack History" << std::endl;
        std::cout << "3. Funny cat picture" << std::endl;
        std::cout << "4. Exit\n" << std::endl;
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
                // Manage active attacks
                this->db->print_history();
                break;
            case 3:
                funnyCatPicture();
                break;
            case 4:
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
                
                // Log the attack
                this->db->log_attack("SYN Flood", "192.168.x.x (You)", target_ip_str);

                t.detach(); // Allows the menu to keep running while attack happens
                std::cout << "[+] Attack launched!" << std::endl;
                break;
                
            }
            case 2: {
                // 1. CHECK: Is it already running?
                if (this->arp_tool.attacking) {
                    std::cout << "\n[!] Stopping ARP Spoofing..." << std::endl;
                    this->arp_tool.attacking = false;
                    
                    // Give the thread a moment to finish its last loop
                    std::this_thread::sleep_for(std::chrono::seconds(2)); 
                    std::cout << "[+] Attack Stopped Successfully." << std::endl;
                } 
                // 2. START: It's not running, so let's configure and start it.
                else {
                    const char* iface;
                    std::cout << "Getting current device's interface..." << std::endl;
                    sleep(WAITING_TIME);
                    iface = helper.get_iface();
                    std::cout << "Your current interface is: " << iface << std::endl;

                    std::string target_ip_str;
                    std::cout << "Input the target IP (Victim's IP): ";
                    std::cin >> target_ip_str;

                    std::string spoof_ip_str;
                    std::cout << "Input the spoofed IP (Gateway/Router IP): ";
                    std::cin >> spoof_ip_str;

                    std::cout << "\n[+] Launching ARP Spoof in background..." << std::endl;

                    // THREAD SAFETY CRITICAL:
                    // We capture 'this' to access arp_tool.
                    // We capture 'target_ip_str' and 'spoof_ip_str' by VALUE (copy) 
                    // so the thread has its own copy of the strings even after this function ends.
                    std::thread attack_thread([this, iface, target_ip_str, spoof_ip_str]() {
                        this->arp_tool.send_arp_spoof(iface, target_ip_str.c_str(), spoof_ip_str.c_str());
                    });
                    
                    attack_thread.detach(); // Detach allows it to run while we go back to menu
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