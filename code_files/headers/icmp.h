#ifndef ICMP_H
#define ICMP_H

#include <iostream> // Input/output streams
#include <cstring> // C-style string functions
#include <cstdlib> // General utilities: atoi, exit
#include <unistd.h> // POSIX API: close, usleep
#include <sys/socket.h> // Socket functions and data structures
#include <netinet/ip.h> // IP header
#include <netinet/ip_icmp.h> // ICMP header
#include <arpa/inet.h>
#include <vector> // vector data type

class ICMP {
public:
    ICMP();
    void send_icmp_flood(const char* target_ip, int packet_count);

private:
    // The standard Internet Checksum algorithm
    unsigned short checksum(void* b, int len);
};

#endif