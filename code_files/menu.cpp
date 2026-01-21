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
                
                // 1. Get your real IP
                const char* iface = helper.get_iface();
                std::string my_ip = helper.get_local_ip(iface);
                // 2. Create the source string (e.g., "192.168.37.132 (You)")
                std::string source_log = my_ip + " (You)";
                // 3. Log it
                this->db->log_attack("SYN Flood", source_log, target_ip_str);

                t.detach(); // Allows the menu to keep running while attack happens
                std::cout << "[+] Attack launched!" << std::endl;
                break;
                
            }
            case 2: {
                if (this->arp_tool.attacking) {
                    this->arp_tool.attacking = false;
                    std::cout << "\n[!] Stopping Attack..." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                } 
                else {
                    // 1. Get Interface
                    const char* iface = helper.get_iface();
                    std::string iface_str = std::string(iface);
                    std::cout << "Interface: " << iface_str << std::endl;

                    // 2. Get Target IP (The ONLY manual input)
                    std::string target_ip_str;
                    std::cout << "Target IP: ";
                    std::cin >> target_ip_str;

                    // 3. Auto-Detect Gateway IP
                    std::cout << "[*] Detecting Gateway IP... ";
                    std::string spoof_ip_str = this->arp_tool.get_default_gateway(iface);
                    if (spoof_ip_str.empty()) {
                        std::cout << "Failed! Enter manually: ";
                        std::cin >> spoof_ip_str;
                    } else {
                        std::cout << spoof_ip_str << " (Found)" << std::endl;
                    }

                    // 4. Auto-Detect Target MAC
                    std::cout << "[*] Resolving Target MAC... ";
                    std::string target_mac_str = helper.get_mac_from_ip(target_ip_str);
                    
                    if (target_mac_str.empty()) {
                        std::cout << "Failed to resolve MAC! Is host alive?\n";
                        std::cout << "Enter MAC manually (e.g. 00:11:22:33:44:55): ";
                        std::cin >> target_mac_str;
                    } else {
                        std::cout << target_mac_str << " (Found)" << std::endl;
                    }

                    std::cout << "\n[+] Launching Stealth Unicast Attack..." << std::endl;

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