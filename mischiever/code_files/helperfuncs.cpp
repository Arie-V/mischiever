#include "helperfuncs.h"

void HelperFunctions::clearScreen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void HelperFunctions::displayImage(const char* filename) {
    std::string command = "xdg-open " + std::string(filename);
    system(command.c_str());
}