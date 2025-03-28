#ifndef HELPERFUNCS_HPP
#define HELPERFUNCS_HPP
#include <cstdlib>

class HelperFunctions {
    public:
        virtual void clearScreen(){
            #ifdef _WIN32
                system("cls");
            #else
                system("clear");
            #endif
        }

};

#endif
