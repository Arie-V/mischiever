#ifndef ICMP_H
#define ICMP_H

#include <string>
#include <thread>
#include <atomic>
#include <vector>

#include "../headers/attack_module.h"

class ICMP : public AttackModule {
public:
    ICMP() : packet_count(-1), stop_flag(false) {}
    ~ICMP();

    // Implementation of the AttackModule interface
    void run(Session* session) override;
    void stop() override;
    std::string get_name() override;

    // Specific setter for this module's parameters
    void set_options(int count);

private:
    // Module-specific parameters
    int packet_count;

    // Threading and control
    std::thread attack_thread;
    std::atomic<bool> stop_flag;

    // Private helper for the attack logic
    void flood_loop(std::string target_ip);

    // The standard Internet Checksum algorithm
    unsigned short checksum(void* b, int len);
};

#endif // ICMP_H
