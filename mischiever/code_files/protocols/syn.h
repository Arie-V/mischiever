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
};

#endif // SYN_H