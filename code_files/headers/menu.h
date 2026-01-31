#ifndef MENU_H
#define MENU_H

#include <vector>
#include <memory>
#include <string>

#include "session.h"
#include "attack_module.h"
#include "helperfuncs.h"

class Menu {
public:
    Menu();
    ~Menu();

    // The main entry point to run the application
    void run();

private:
    // Application state
    Session session;
    // Collection of available attack modules
    std::vector<std::unique_ptr<AttackModule>> attack_modules;

    // Main menu and header
    void print_logo();
    void display_main_menu_header();
    void display_main_menu();

    // Sub-menu navigation
    void show_attack_modules_menu();
    void show_floods_menu();
    void show_spoofings_menu();
    void show_attack_history();
    void show_dos_menu();

    // Core logic
    void show_target_config_menu();  // Target configuration menu
    void view_target_config();       // Option 1
    void set_target_config();      // Option 2
    void delete_target_config();     // Option 3
    void run_selected_attack(AttackModule* attack);
};

#endif // MENU_H