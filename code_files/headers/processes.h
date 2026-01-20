#ifndef PROCESSES_H
#define PROCESSES_H

#include <thread>
#include <vector>

class Processes {
    public:
        // Constructor
        Processes();
        // Destructor
        ~Processes();

        // Function to join all threads
        void join_threads();
        // Function to clear all threads
        void clear_threads();
        // Function to add a thread to the vector
        void add_thread(std::thread* thread);
        // Function to remove a thread from the vector
        void remove_thread(int index);
        // Function to get all threads
        const std::vector<std::thread*>& get_threads() const;

    private:
        // Dynamic array to store all threads
        std::vector<std::thread*> threads;
};

#endif // PROCESSES_H