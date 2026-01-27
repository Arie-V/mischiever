#include "headers/processes.h"

Processes::Processes() {}

Processes::~Processes() {
    clear_threads(); // Ensure all threads are cleaned up
}

void Processes::join_threads() {
    // Use size_t for the loop counter to match the type of std::vector::size(), avoiding a compiler warning.
    for (size_t i = 0; i < this->threads.size(); i++) {
        if (this->threads[i] && this->threads[i]->joinable()) {
            this->threads[i]->join(); // Use -> to call join() on the pointer
        }
    }
}

void Processes::clear_threads() {
    // Use size_t for the loop counter to match the type of std::vector::size(), avoiding a compiler warning.
    for (size_t i = 0; i < this->threads.size(); i++) {
        delete this->threads[i]; // Free the dynamically allocated memory
    }
    this->threads.clear();
}

void Processes::add_thread(std::thread* thread) {
    this->threads.push_back(thread); // Store the pointer in the vector
}

void Processes::remove_thread(int index) {
    // Check if the index is non-negative and within the bounds of the vector.
    // The index is cast to size_t to safely compare with this->threads.size() and avoid a signed/unsigned mismatch warning.
    if (index >= 0 && static_cast<size_t>(index) < this->threads.size()) {
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