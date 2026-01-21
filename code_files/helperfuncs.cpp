#include "headers/helperfuncs.h"
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

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

std::string HelperFunctions::get_local_ip(const char* iface) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    
    // Set the interface name (e.g., "eth0")
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

    std::string ip_str = "";

    // Ask the kernel for the address
    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
        struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
        ip_str = inet_ntoa(ipaddr->sin_addr);
    } else {
        ip_str = "Unknown";
    }

    close(fd);
    return ip_str;
}

std::string HelperFunctions::get_mac_from_ip(std::string ip_addr) {
    // 1. Send a single silent ping to force the OS to resolve the MAC
    std::string cmd = "ping -c 1 -W 1 " + ip_addr + " > /dev/null 2>&1";
    system(cmd.c_str());

    // 2. Read the system ARP cache
    std::ifstream arp_file("/proc/net/arp");
    if (!arp_file.is_open()) return "";

    std::string line;
    // Skip header line
    std::getline(arp_file, line); 

    while (std::getline(arp_file, line)) {
        std::stringstream ss(line);
        std::string ip, hw_type, flags, mac, mask, dev;
        
        // Columns: IP | HW_Type | Flags | MAC | Mask | Device
        ss >> ip >> hw_type >> flags >> mac >> mask >> dev;

        if (ip == ip_addr) {
            // Found it!
            if (mac == "00:00:00:00:00:00") return ""; // Failed resolve
            return mac;
        }
    }
    return ""; // Not found
}