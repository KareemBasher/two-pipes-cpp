#include <unistd.h>
#include <iostream>
#include <cstring>

int main() {
    // Create pipes
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        std::cerr << "Failed to create pipes" << std::endl;
        return 1;
    }

    // Create a child process
    pid_t child_pid = fork();
    if (child_pid == -1) {
        std::cerr << "Failed to create child process" << std::endl;
        return 1;
    }

    if (child_pid == 0) {
        // This is the child process

        // Close the write end of pipe1 and the read end of pipe2
        close(pipe1[1]);
        close(pipe2[0]);

        // Read from the read end of pipe1
        constexpr size_t BUF_SIZE = 100;
        char buf[BUF_SIZE];
        ssize_t num_read = read(pipe1[0], buf, BUF_SIZE);
        if (num_read == -1) {
            std::cerr << "Failed to read from pipe" << std::endl;
            return 1;
        }

        // Print menu and ask user for input
        std::cout << "Enter 1 to reverse the message or 2 to concatenate a message to the message: " << std::endl;
        int option;
        std::cin >> option;

        // Consume the newline character
        std::cin.get();

        if (option == 1) {
            // Reverse the message
            for (size_t i = 0; i < num_read / 2; ++i) {
                std::swap(buf[i], buf[num_read - i - 1]);
            }
        } else if (option == 2) {
            // Concatenate a message to the original
            // message
            std::cout << "Enter the concatenated message: ";
            std::string concatenated_message;
            std::getline(std::cin, concatenated_message);

            strncat(buf, " ", BUF_SIZE - num_read - 1);
            strncat(buf, concatenated_message.c_str(), BUF_SIZE - num_read - 1);
            num_read += concatenated_message.size() + 1;  // Update num_read to reflect the actual length of the modified string
        } else {
            std::cout << "Invalid option" << std::endl;
            return 1;
        }

        // Write the modified message to the write end of pipe2
        if (write(pipe2[1], buf, num_read) == -1) {
            std::cerr << "Failed to write to pipe" << std::endl;
            return 1;
        }
    } else {
        // This is the parent process

        // Close the read end of pipe1 and the write end of pipe2
        close(pipe1[0]);
        close(pipe2[1]);

        // Ask the user for the message
        std::cout << "Enter a message: ";
        std::string message;
        std::getline(std::cin, message);

        // Write the message to the write end of pipe1
        if (write(pipe1[1], message.c_str(), message.size()) == -1) {
            std::cerr << "Failed to write to pipe" << std::endl;
            return 1;
        }

        // Read from the read end of pipe2
        constexpr size_t BUF_SIZE = 100;
        char buf[BUF_SIZE];
        ssize_t num_read = read(pipe2[0], buf, BUF_SIZE);
        if (num_read == -1) {
            std::cerr << "Failed to read from pipe" << std::endl;
            return 1;
        }

        // Print the modified message
        std::cout << buf << std::endl;
    }

    return 0;
}
