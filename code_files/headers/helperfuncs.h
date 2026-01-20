#ifndef HELPERFUNCS_H
#define HELPERFUNCS_H
#include <cstdlib>
#include <string>
#include <vector>
#include <cstring>
#include <ifaddrs.h>
#include <net/if.h>

class HelperFunctions {
    public:
    void clearScreen();
    void displayImage(const char* filename);
    const char* get_iface();
};

#endif
