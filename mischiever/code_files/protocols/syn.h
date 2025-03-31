#ifndef SYN_H
#define SYN_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <ctime>

class SYN {
    public:
        void syn_flood(const char* target_ip, int target_port, int packet_count);
    private:
    
    // Pseudo-header structure for TCP checksum calculation
    struct PseudoHeader {
        uint32_t source_ip;
        uint32_t dest_ip;
        uint8_t reserved;
        uint8_t protocol;
        uint16_t tcp_length;
    };

    unsigned short checksum(void* data, int len);
};

#endif // SYN_H