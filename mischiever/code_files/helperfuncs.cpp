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

const char* HelperFunctions::get_iface() {
    struct ifaddrs *ifaddr, *ifa;
    const char* iface_name = "unknown";

    // Get list of network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return iface_name;
    }

    // Iterate through interfaces
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr && (ifa->ifa_flags & IFF_UP) && !(ifa->ifa_flags & IFF_LOOPBACK)) {
            iface_name = ifa->ifa_name; // Get the first active non-loopback interface
            break;
        }
    }

    freeifaddrs(ifaddr); // Free memory
    return iface_name;
}