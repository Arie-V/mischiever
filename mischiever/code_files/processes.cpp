#include "processes.h"

Processes::Processes() {}

Processes::~Processes() {
    clear_threads(); // Ensure all threads are cleaned up
}

void Processes::join_threads() {
    for (int i = 0; i < this->threads.size(); i++) {
        if (this->threads[i] && this->threads[i]->joinable()) {
            this->threads[i]->join(); // Use -> to call join() on the pointer
        }
    }
}

void Processes::clear_threads() {
    for (int i = 0; i < this->threads.size(); i++) {
        delete this->threads[i]; // Free the dynamically allocated memory
    }
    this->threads.clear();
}

void Processes::add_thread(std::thread* thread) {
    this->threads.push_back(thread); // Store the pointer in the vector
}

void Processes::remove_thread(int index) {
    if (index >= 0 && index < this->threads.size()) {
        if (this->threads[index] && this->threads[index]->joinable()) {
            this->threads[index]->join(); // Ensure the thread is joined before removing
        }
        delete this->threads[index]; // Free the memory
        this->threads.erase(this->threads.begin() + index);
    }
}

const std::vector<std::thread*>& Processes::get_threads() const {
    return this->threads; // Return a const reference to avoid copying
}