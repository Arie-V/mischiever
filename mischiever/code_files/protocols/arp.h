#ifndef ARP_H
#define ARP_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_packet.h>
#include <thread>
#include <mutex>

class ARP {
    public:
        // Function to send an ARP spoofing packet
        void send_arp_spoof(const char* iface, const char* target_ip, const char* spoof_ip);
    private:

    // Structure for ARP header
    struct arp_header {
        uint16_t htype;     // Hardware type (Ethernet = 1)
        uint16_t ptype;     // Protocol type (IPv4 = 0x0800)
        uint8_t hlen;       // Hardware address length (MAC = 6)
        uint8_t plen;       // Protocol address length (IPv4 = 4)
        uint16_t oper;      // Operation (1 = Request, 2 = Reply)
        uint8_t sha[6];     // Sender MAC address
        uint8_t spa[4];     // Sender IP address
        uint8_t tha[6];     // Target MAC address
        uint8_t tpa[4];     // Target IP address
    };

    // Function to get the MAC address of the attacker's network interface
    void get_mac_address(const char* iface, uint8_t* mac);
};

#endif // ARP_H