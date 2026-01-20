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
#include <chrono>   
#include <mutex>
#include <atomic> // Need this for the stop flag
#include <cstdint> // standard types like uint8_t

class ARP {
    public:
        // Control flag to stop the attack
        std::atomic<bool> attacking;

        // Constructor to initialize flag
        ARP() { attacking = false; }
    
        // Function to send an ARP spoofing packet
        void send_arp_spoof(const char* iface, const char* target_ip, const char* spoof_ip);
    private:

    // Structure for ARP header
        struct __attribute__((packed)) arp_header {
            uint16_t htype;
            uint16_t ptype;
            uint8_t hlen;
            uint8_t plen;
            uint16_t oper;
            uint8_t sha[6];
            uint8_t spa[4];
            uint8_t tha[6];
            uint8_t tpa[4];
        };

    // Function to get the MAC address of the attacker's network interface
    void get_mac_address(const char* iface, uint8_t* mac);
};

#endif // ARP_H