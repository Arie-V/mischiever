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

    // Load all available attack modules
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
        if (std::cin.fail()) {
            choice = -1; // Invalid input
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: show_attack_modules_menu(); break;
            case 2: show_attack_history(); break;
            case 3: session.helper->displayImage("misc/cat.jpg"); break; // Easter egg
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
    sleep(2);
}

void Menu::display_main_menu_header() {
    session.helper->clearScreen();
    std::cout << C_BLUE << "======================================================================" << C_RESET << std::endl;
    
    // A clean, multi-line format guarantees perfect alignment
    std::cout << C_CYAN << "  Interface : " << C_RESET 
              << (session.interface.empty() ? C_YELLOW "[Not Set]" : C_GREEN + session.interface) << C_RESET;

    std::cout << C_CYAN << "  Target IP : " << C_RESET
              << (session.target_ip.empty() ? C_YELLOW "[Not Set]" : C_GREEN + session.target_ip) << C_RESET;

    std::cout << C_CYAN << "  Gateway IP: " << C_RESET
              << (session.gateway_ip.empty() ? C_YELLOW "[Not Set]" : C_GREEN + session.gateway_ip) << C_RESET << std::endl;

    std::cout << C_BLUE << "======================================================================" << C_RESET << std::endl;
}

void Menu::display_main_menu() {
    display_main_menu_header();
    std::cout << C_BOLD << "            MAIN MENU                   " << C_RESET << std::endl;
    std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
    std::cout << C_GREEN << "[1]" << C_RESET << " Attack Modules" << std::endl;
    std::cout << C_GREEN << "[2]" << C_RESET << " Attack History" << std::endl;
    std::cout << C_GREEN << "[3]" << C_RESET << " The Cat" << std::endl;
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
        
        if (selected_attack) {
            set_target_config();
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
    std::cout << C_BOLD << "           ATTACK HISTORY               " << C_RESET << std::endl;
    std::cout << C_BLUE << "======================================================================" << C_RESET << std::endl;
    session.db->print_history();
    std::cout << C_BLUE << "======================================================================" << C_RESET << std::endl;
    std::cout << "\nPress Enter to return..." << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// --- Core Logic ---

void Menu::set_target_config() {
    display_main_menu_header();
    std::cout << C_BOLD << "         TARGET CONFIGURATION           " << C_RESET << std::endl;
    std::cout << C_BLUE << "========================================" << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Press Enter to keep current value." << C_RESET << std::endl;
    std::cout << C_YELLOW << "[!] Type 'resolve' in MAC fields to auto-discover." << C_RESET << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string temp;

    std::cout << C_CYAN << "Target IP [" << (session.target_ip.empty() ? "None" : session.target_ip) << "]: " << C_RESET;
    std::getline(std::cin, temp);
    if (!temp.empty()) session.target_ip = temp;

    std::cout << C_CYAN << "Target MAC [" << (session.target_mac.empty() ? "None" : session.target_mac) << "]: " << C_RESET;
    std::getline(std::cin, temp);
    if (!temp.empty()) {
        if (temp == "resolve") {
            std::cout << C_YELLOW << "[*] Resolving Target MAC... " << C_RESET;
            std::string mac = session.helper->get_mac_from_ip(session.target_ip);
            if (!mac.empty()) {
                session.target_mac = mac;
                std::cout << C_GREEN << mac << " (Found)" << C_RESET << std::endl;
            } else {
                std::cout << C_RED << "Failed! Could not resolve." << C_RESET << std::endl;
            }
        } else {
           session.target_mac = temp;
        }
    }
    
    std::cout << C_CYAN << "Gateway IP [" << (session.gateway_ip.empty() ? "None" : session.gateway_ip) << "]: " << C_RESET;
    std::getline(std::cin, temp);
    if (!temp.empty()) session.gateway_ip = temp;

    std::cout << C_CYAN << "Gateway MAC [" << (session.gateway_mac.empty() ? "None" : session.gateway_mac) << "]: " << C_RESET;
    std::getline(std::cin, temp);
     if (!temp.empty()) {
        if (temp == "resolve") {
            std::cout << C_YELLOW << "[*] Resolving Gateway MAC... " << C_RESET;
            std::string mac = session.helper->get_mac_from_ip(session.gateway_ip);
            if (!mac.empty()) {
                session.gateway_mac = mac;
                std::cout << C_GREEN << mac << " (Found)" << C_RESET << std::endl;
            } else {
                std::cout << C_RED << "Failed! Could not resolve." << C_RESET << std::endl;
            }
        } else {
           session.gateway_mac = temp;
        }
    }
    std::cout << C_GREEN << "\nConfiguration updated. Returning to menu." << C_RESET << std::endl;
    sleep(2);
}

void Menu::run_selected_attack(AttackModule* attack) {
    if (!attack) return;
    if (session.target_ip.empty()) {
        std::cerr << C_RED << "Target IP is not set! Please configure it first." << C_RESET << std::endl;
        sleep(2);
        return;
    }

    // Log the attack before running
    std::string my_ip = session.helper->get_local_ip(session.interface.c_str());
    std::string source_log = my_ip.empty() ? "Unknown (You)" : my_ip + " (You)";
    session.db->log_attack(attack->get_name(), source_log, session.target_ip);
    
    attack->run(&session);
    
    std::cout << C_YELLOW << "\nAttack is running. Press [Enter] to stop it." << C_RESET << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get(); 

    attack->stop();
    std::cout << C_GREEN << "Attack stopped." << C_RESET << std::endl;
    sleep(1);
}
