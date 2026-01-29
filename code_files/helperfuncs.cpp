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

bool HelperFunctions::is_valid_ip(const std::string& ip) {
    // Regex for IPv4 address
    const std::regex pattern("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
    return std::regex_match(ip, pattern);
}

bool HelperFunctions::is_valid_mac(const std::string& mac) {
    // Regex for MAC address
    const std::regex pattern("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
    return std::regex_match(mac, pattern);
}

/*
 * FUNCTION: scan_local_network
 * ----------------------------
 * Performs a fast discovery of active hosts on the Local Area Network (LAN).
 *
 * HOW IT WORKS:
 * 1. "The Shout": Executes a shell command to send ICMP Echo Requests (Pings)
 * to every IP in the subnet (1-254) simultaneously in background threads.
 * 2. "The Echo": When a device replies to the ping, the Linux Kernel automatically
 * updates its ARP Cache with that device's MAC address.
 * 3. "The Harvest": We parse /proc/net/arp to read these new entries.
 *
 * NOTE: This "Ping Sweep" technique allows us to discover hosts without writing
 * complex raw socket code for ARP Request/Reply handling.
 */
void HelperFunctions::scan_local_network(const char* interface) {
    std::string local_ip = get_local_ip(interface);
    if (local_ip.empty()) {
        std::cerr << "Error: Could not get local IP for interface " << interface << std::endl;
        return;
    }

    // 1. Calculate the Subnet (e.g., "192.168.1.")
    std::string subnet = local_ip.substr(0, local_ip.find_last_of('.') + 1);

    std::cout << "\n" << C_YELLOW << "[*] Scanning " << subnet << "0/24 (approx 2s)..." << C_RESET << std::endl;

    // 2. The "Bash Sweep"
    std::string cmd = "for i in $(seq 1 254); do ping -c 1 -W 1 " + subnet + "$i > /dev/null & done; wait";
    system(cmd.c_str());

    // 3. Read the Kernel ARP Table
    std::ifstream arp_file("/proc/net/arp");
    std::string line;
    
    // Header with Blue Bars
    std::cout << C_BLUE << "==========================================" << C_RESET << std::endl;
    std::cout << C_BOLD << " IP ADDRESS      MAC ADDRESS" << C_RESET << std::endl;
    std::cout << C_BLUE << "==========================================" << C_RESET << std::endl;

    // Skip the file header line
    std::getline(arp_file, line);

    bool found = false;
    while(std::getline(arp_file, line)) {
        std::stringstream ss(line);
        std::string ip, hw_type, flags, mac, mask, dev;
        
        ss >> ip >> hw_type >> flags >> mac >> mask >> dev;

        // Filter: Must match our interface AND not be incomplete (0x0)
        if (dev == interface && flags != "0x0") {
            // Print IP in Green, MAC in default
            std::cout << " " << C_GREEN << std::left << std::setw(15) << ip << C_RESET << " " << mac << std::endl;
            found = true;
        }
    }
    
    if (!found) {
        std::cout << C_YELLOW << " No active hosts found (besides you)." << C_RESET << std::endl;
    }
    std::cout << C_BLUE << "==========================================" << C_RESET << "\n" << std::endl;
}

/*
 * FUNCTION: get_default_gateway_ip
 * --------------------------------
 * Retrieves the Default Gateway (Router) IP by reading the Linux kernel's
 * routing table located at /proc/net/route.
 *
 * HOW IT WORKS:
 * 1. Opens the file which contains active routes in Hexadecimal format.
 * 2. Scans for a Destination of "00000000".
 * - In networking, 0.0.0.0 with mask 0 is the "Default Route" (The Internet).
 * 3. Extracts the "Gateway" column from that row (e.g., "0100A8C0").
 * 4. Converts the Little Endian Hex into a standard IP string (e.g., "192.168.0.1").
 *
 * NOTE: This is a PASSIVE operation. It reads a local file and does NOT send
 * any packets (ARP/ICMP) to the network.
 */
std::string HelperFunctions::get_default_gateway_ip() {
    std::ifstream route_file("/proc/net/route");
    std::string line;
    
    // Skip the header line (Iface Destination Gateway ...)
    if (!std::getline(route_file, line)) return ""; 

    while (std::getline(route_file, line)) {
        std::stringstream ss(line);
        std::string iface, dest, gateway_hex;
        
        // Read the first three columns: Iface, Destination, Gateway
        ss >> iface >> dest >> gateway_hex;

        // Destination "00000000" means this is the Default Gateway
        if (dest == "00000000") {
            unsigned int ip_val;
            std::stringstream ss_hex;
            ss_hex << std::hex << gateway_hex;
            ss_hex >> ip_val;
            
            struct in_addr ip_addr;
            ip_addr.s_addr = ip_val;
            return inet_ntoa(ip_addr);
        }
    }
    return "";
}