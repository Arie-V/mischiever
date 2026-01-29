#include <iostream>
#include <limits>
#include <unistd.h> // for sleep()
#include <cstdlib>  // for srand(), time()
#include <ctime>    // for time()
#include <iomanip> // For std::setw

#include "headers/menu.h"
#include "headers/syn.h"
#include "headers/arp.h"
#include "headers/icmp.h"

// --- ANSI Color Codes ---
#define C_RESET       "\033[0m"
#define C_RED         "\033[1;31m"
#define C_GREEN       "\033[1;32m"
#define C_YELLOW      "\033[1;33m"
#define C_BLUE        "\033[1;34m"
#define C_MAGENTA     "\033[1;35m"
#define C_CYAN        "\033[1;36m"
#define C_BOLD        "\033[1m"

// --- Constructor & Destructor ---
Menu::Menu() {
    // Seed random number generator
    srand(time(0));
}

Menu::~Menu() {}

// --- Main Application Logic ---
void Menu::run() {
    print_logo();

    // [PLATFORM ARCHITECTURE]
    // Instead of creating tools on demand, we load them all into an "Inventory" (vector) at startup.
    // This allows us to add 50 new attacks without changing the main loop logic.
    // We use unique_ptr to handle memory automatically (no more manual 'delete').
    attack_modules.push_back(std::unique_ptr<SYN>(new SYN()));
    attack_modules.push_back(std::unique_ptr<ARP>(new ARP()));
    attack_modules.push_back(std::unique_ptr<ICMP>(new ICMP()));

    // Set default interface automatically if possible
    const char* default_iface = session.helper->get_iface();
    if (default_iface) {
        session.interface = std::string(default_iface);
    } else {
        std::cout << C_RED << "Could not detect default interface. Please set one in Target Configuration." << C_RESET << std::endl;
        sleep(2);
    }
    
    int choice = -1;
    while (choice != 4) {
        display_main_menu();
        
        std::cin >> choice;
        // Handle invalid input
        if (std::cin.fail()) {
            choice = -1; // Invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: show_attack_modules_menu(); break;
            case 2: show_attack_history(); break;
            case 3: set_target_config(); break;
            case 69: session.helper->displayImage("misc/cat.jpg"); break; // Easter egg
            case 777: session.helper->displayImage("misc/cat2.png"); break; // Easter egg
            case 4: break; // Exit
            default:
                std::cout << C_RED << "Invalid choice. Please try again." << C_RESET << std::endl;
                sleep(1);
                break;
        }
    }

    std::cout << C_YELLOW << "Exiting Mischiever." << C_RESET << std::endl;
}

// --- Menu Display Functions ---

void Menu::print_logo() {
    session.helper->clearScreen();
    std::cout << C_MAGENTA << R"(
    __  ____       __    _                     
   /  |/  (_)_____/ /_  (_)__ _   _____  _____
  / /|_/ / / ___/ __ \/ / _ \ | / / _ \/ ___/
 / /  / / (__  ) /_/ / /  __/ |/ /  __/ /    
/_/  /_/_/____/_.___/_/\___/|___/\___/_/     
                                             
    )" << C_RESET << std::endl;
    std::cout << C_CYAN << "      The Network Swiss Army Knife" << C_RESET << "\n" << std::endl;
    sleep(1);
}

void Menu::display_main_menu_header() {
    // Clear the screen first
    session.helper->clearScreen();
    
    // A nice wide separator
    std::cout << C_BLUE << "================================================================================" << C_RESET << std::endl;

    // Helper lambda to handle "None" vs "Value" coloring cleanly
    auto colorize = [](const std::string& val) {
        // If empty -> Yellow "None", If set -> Green Value
        return val.empty() ? (std::string(C_YELLOW) + "None" + C_RESET) : (std::string(C_GREEN) + val + C_RESET);
    };

    // The Single Line Dashboard
    // Uses Blue Pipes " | " to visually separate the sections
    std::cout << C_BOLD << " Interface: " << C_RESET << colorize(session.interface) 
              << C_BLUE << "  |  " << C_RESET 
              << C_BOLD << "Target IP: " << C_RESET << colorize(session.target_ip)
              << C_BLUE << "  |  " << C_RESET 
              << C_BOLD << "Gateway IP: " << C_RESET << colorize(session.gateway_ip) 
              << std::endl;

    std::cout << C_BLUE << "================================================================================" << C_RESET << std::endl;
}

void Menu::display_main_menu() {
    display_main_menu_header();
    std::cout << C_BOLD << "            MAIN MENU                   " << C_RESET << std::endl;
    std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
    std::cout << C_GREEN << "[1]" << C_RESET << " Attack Modules" << std::endl;
    std::cout << C_GREEN << "[2]" << C_RESET << " Attack History" << std::endl;
    std::cout << C_GREEN << "[3]" << C_RESET << " Target Configuration" << std::endl;
    std::cout << C_GREEN << "[4]" << C_RESET << " Exit" << std::endl;
    std::cout << std::endl << C_BOLD << "mischiever > " << C_RESET;
}

void Menu::show_attack_modules_menu() {
    int choice = -1;
    while (choice != 3) {
        display_main_menu_header();
        std::cout << C_BOLD << "           ATTACK MODULES               " << C_RESET << std::endl;
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_GREEN << "[1]" << C_RESET << " Floods" << std::endl;
        std::cout << C_GREEN << "[2]" << C_RESET << " Spoofings" << std::endl;
        std::cout << C_GREEN << "[3]" << C_RESET << " Back" << std::endl;
        std::cout << std::endl << C_BOLD << "mischiever/modules > " << C_RESET;

        std::cin >> choice;
        if (std::cin.fail()) {
            choice = -1;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: show_floods_menu(); break;
            case 2: show_spoofings_menu(); break;
            case 3: break;
            default:
                std::cout << C_RED << "Invalid choice." << C_RESET << std::endl;
                sleep(1);
                break;
        }
    }
}

void Menu::show_floods_menu() {
    int choice = -1;
    while (choice != 3) {
        display_main_menu_header();
        std::cout << C_BOLD << "           FLOOD ATTACKS                " << C_RESET << std::endl;
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_GREEN << "[1]" << C_RESET << " SYN Flood" << std::endl;
        std::cout << C_GREEN << "[2]" << C_RESET << " ICMP Ping Flood" << std::endl;
        std::cout << C_GREEN << "[3]" << C_RESET << " Back" << std::endl;
        std::cout << std::endl << C_BOLD << "mischiever/modules/floods > " << C_RESET;
        
        std::cin >> choice;
        if (std::cin.fail()) {
            choice = -1;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        AttackModule* selected_attack = nullptr;
        switch (choice) {
            case 1: // SYN
                for (const auto& mod : attack_modules) {
                    if (mod->get_name() == "SYN Flood") selected_attack = mod.get();
                }
                break;
            case 2: // ICMP
                for (const auto& mod : attack_modules) {
                    if (mod->get_name() == "ICMP Flood") selected_attack = mod.get();
                }
                break;
            case 3: return;
            default:
                std::cout << C_RED << "Invalid choice." << C_RESET << std::endl;
                sleep(1);
                continue;
        }
        
        // Smoother now
        if (selected_attack) {
            // Only ask for config if we genuinely don't have a target yet
            if(session.target_ip.empty()) {
                std::cout << C_YELLOW << "[!] Target not set. Redirecting to configuration..." << C_RESET << std::endl;
                set_target_config();
            }
            run_selected_attack(selected_attack);
        }
    }
}

void Menu::show_spoofings_menu() {
    int choice = -1;
    while (choice != 2) {
        display_main_menu_header();
        std::cout << C_BOLD << "           SPOOFING ATTACKS             " << C_RESET << std::endl;
        std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
        std::cout << C_GREEN << "[1]" << C_RESET << " ARP Spoof" << std::endl;
        std::cout << C_GREEN << "[2]" << C_RESET << " Back" << std::endl;
        std::cout << std::endl << C_BOLD << "mischiever/modules/spoofings > " << C_RESET;

        std::cin >> choice;
        if (std::cin.fail()) {
            choice = -1;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        if (choice == 1) {
            AttackModule* arp_attack = nullptr;
            for (const auto& mod : attack_modules) {
                if (mod->get_name() == "ARP Spoof") arp_attack = mod.get();
            }
            if (arp_attack) {
                set_target_config();
                run_selected_attack(arp_attack);
            }
        } else if (choice != 2) {
             std::cout << C_RED << "Invalid choice." << C_RESET << std::endl;
             sleep(1);
        }
    }
}

void Menu::show_attack_history() {
    display_main_menu_header();
    std::cout << C_BOLD << "                            ATTACK HISTORY               " << C_RESET << std::endl;
    std::cout << C_BLUE << "================================================================================================================================" << C_RESET << std::endl;
    session.db->print_history();
    std::cout << C_BLUE << "================================================================================================================================" << C_RESET << std::endl;
    std::cout << "\nPress Enter to return..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Target configuration menu with input validation and auto-resolve features
void Menu::set_target_config() {
    display_main_menu_header();
    std::cout << C_BOLD << "         TARGET CONFIGURATION           " << C_RESET << std::endl;
    std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Press Enter to keep current value." << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Type 'find' to scan LAN IP's or detect Gateway's IP." << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Type 'resolve' to auto-discover MAC addresses." << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Type 'q' to quit or 'd' to clear config.\n" << C_RESET << std::endl;
    
    // Clear buffer before starting
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string temp;
    bool valid_input;

    // --- HELPER LAMBDA TO HANDLE EXIT COMMANDS ---
    auto check_exit = [&](const std::string& input) -> bool {
        if (input == "q") {
            std::cout << C_GREEN << "Exiting configuration..." << C_RESET << std::endl;
            sleep(1);
            return true; 
        }
        if (input == "d") {
            session.target_ip.clear();
            session.target_mac.clear();
            session.gateway_ip.clear();
            session.gateway_mac.clear();
            std::cout << C_RED << "Configuration deleted." << C_RESET << std::endl;
            sleep(1);
            return true;
        }
        return false;
    };

    // 1. Target IP
    do {
        valid_input = true;
        std::cout << C_CYAN << "Target IP [" << (session.target_ip.empty() ? "None" : session.target_ip) << "]: " << C_RESET;
        std::getline(std::cin, temp);

        if (check_exit(temp)) return; // Check for 'q' or 'd'

        // Special command to run network scan inside the configuration
        if (temp == "find") {
            // 1. Check if we are missing the Interface
            if (session.interface.empty()) {
                std::cout << C_YELLOW << "[!] Interface not set. Please enter it (e.g., eth0): " << C_RESET;
                std::string manual_iface;
                std::getline(std::cin, manual_iface);

                if (check_exit(manual_iface)) return; // Allow them to cancel

                if (!manual_iface.empty()) {
                    session.interface = manual_iface;
                    std::cout << C_GREEN << "Interface set to: " << session.interface << C_RESET << std::endl;
                } else {
                    std::cout << C_RED << "Scan aborted: No interface provided." << C_RESET << std::endl;
                    valid_input = false; 
                    continue;
                }
            }
            
            // 2. Run the scan
            session.helper->scan_local_network(session.interface.c_str());
            
            valid_input = false; // Loop again so they can type the IP they just found
            continue;
        }

        if (!temp.empty()) {
            if (session.helper->is_valid_ip(temp)) {
                session.target_ip = temp;
            } else {
                std::cout << C_RED << "Invalid IP format." << C_RESET << std::endl;
                valid_input = false;
            }
        }
    } while (!valid_input);

    // 2. Target MAC
    do {
        valid_input = true;
        std::cout << C_CYAN << "Target MAC [" << (session.target_mac.empty() ? "None" : session.target_mac) << "]: " << C_RESET;
        std::getline(std::cin, temp);

        if (check_exit(temp)) return;

        if (!temp.empty()) {
            if (temp == "resolve") {
                if (session.target_ip.empty()) {
                    std::cout << C_RED << "Cannot resolve MAC without Target IP!" << C_RESET << std::endl;
                    valid_input = false; 
                } else {
                    std::cout << C_YELLOW << "[*] Resolving Target MAC... " << C_RESET;
                    std::string mac = session.helper->get_mac_from_ip(session.target_ip);
                    if (!mac.empty()) {
                        session.target_mac = mac;
                        std::cout << C_GREEN << mac << " (Found)" << C_RESET << std::endl;
                    } else {
                        std::cout << C_RED << "Failed! Enter manually." << C_RESET << std::endl;
                        valid_input = false;
                    }
                }
            } else if (session.helper->is_valid_mac(temp)) {
                session.target_mac = temp;
            } else {
                std::cout << C_RED << "Invalid MAC format." << C_RESET << std::endl;
                valid_input = false;
            }
        }
    } while (!valid_input);

    // 3. Gateway IP
    do {
        valid_input = true;
        std::cout << C_CYAN << "Gateway IP [" << (session.gateway_ip.empty() ? "None" : session.gateway_ip) << "]: " << C_RESET;
        std::getline(std::cin, temp);

        if (check_exit(temp)) return;

        // Special command to auto-detect default gateway
        if (temp == "find") {
            std::cout << C_YELLOW << "[*] Detecting Default Gateway... " << C_RESET;
            std::string gw = session.helper->get_default_gateway_ip();
            
            if (!gw.empty()) {
                session.gateway_ip = gw;
                std::cout << C_GREEN << gw << " (Found & Set, Press Enter to confirm)" << C_RESET << std::endl;
                valid_input = false; // Loop again so user sees it in the prompt brackets [10.0.0.1]
            } else {
                std::cout << C_RED << "Failed! Could not find a default route." << C_RESET << std::endl;
                valid_input = false; 
            }
            continue;
        }

        if (!temp.empty()) {
            if (session.helper->is_valid_ip(temp)) {
                session.gateway_ip = temp;
            } else {
                std::cout << C_RED << "Invalid IP format." << C_RESET << std::endl;
                valid_input = false;
            }
        }
    } while (!valid_input);

    // 4. Gateway MAC
    do {
        valid_input = true;
        std::cout << C_CYAN << "Gateway MAC [" << (session.gateway_mac.empty() ? "None" : session.gateway_mac) << "]: " << C_RESET;
        std::getline(std::cin, temp);

        if (check_exit(temp)) return;

        if (!temp.empty()) {
            if (temp == "resolve") {
                if (session.gateway_ip.empty()) {
                    std::cout << C_RED << "Cannot resolve MAC without Gateway IP!" << C_RESET << std::endl;
                    valid_input = false;
                } else {
                    std::cout << C_YELLOW << "[*] Resolving Gateway MAC... " << C_RESET;
                    std::string mac = session.helper->get_mac_from_ip(session.gateway_ip);
                    if (!mac.empty()) {
                        session.gateway_mac = mac;
                        std::cout << C_GREEN << mac << " (Found)" << C_RESET << std::endl;
                    } else {
                        std::cout << C_RED << "Failed! Enter manually." << C_RESET << std::endl;
                        valid_input = false;
                    }
                }
            } else if (session.helper->is_valid_mac(temp)) {
                session.gateway_mac = temp;
            } else {
                std::cout << C_RED << "Invalid MAC format." << C_RESET << std::endl;
                valid_input = false;
            }
        }
    } while (!valid_input);

    std::cout << C_GREEN << "\nConfiguration updated.\n" << C_RESET << std::endl;
    sleep(2);
}

// Dynamic attack runner that takes any AttackModule and runs it using shared Session state
void Menu::run_selected_attack(AttackModule* attack) {
    if (!attack) return;
    // Edge case: Ensure target IP is set
    if (session.target_ip.empty()) {
        std::cerr << C_RED << "Target IP is not set! Please configure it first." << C_RESET << std::endl;
        sleep(2);
        return;
    }

    // Log the attack before running
    // Get local IP for logging by querying the interface with the helper functions we declared inside session
    std::string my_ip = session.helper->get_local_ip(session.interface.c_str());
    // For source_log, if my_ip is empty, use "Unknown (You)" to indicate the attack is from us - otherwise use the actual IP
    std::string source_log = my_ip.empty() ? "Unknown (You)" : my_ip + " (You)";
    // Log the attack with database service inside session, we get the info from here and the session 
    session.db->log_attack(attack->get_name(), source_log, session.target_ip);
    
    // Use the run attack inside the module's class, passing the shared session state
    attack->run(&session);
    
    std::cout << C_YELLOW << "\nAttack is running. Press [Enter] to stop it." << C_RESET << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); 

    attack->stop();
    std::cout << C_GREEN << "Attack stopped." << C_RESET << std::endl;
    sleep(1);
}
