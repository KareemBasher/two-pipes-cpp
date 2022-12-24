# Pipe example in C++

This code demonstrates how to use pipes to communicate between a parent and a child process in C++. The user is prompted to enter a message, which is then sent to the child process through a pipe. The child process reads the message from the pipe and either reverses it or concatenates a message to it, depending on the user's input. The modified message is then sent back to the parent process through a second pipe and is printed to the console.

## How it works

The code uses the `pipe()` function to create two pipes, which are used to communicate between the parent and child processes. The `fork()` function is then used to create a child process, which is a copy of the parent process with its own memory, file descriptors, and other resources.

The child process reads the message from the first pipe using the `read()` function and modifies it as desired. The modified message is then written to the second pipe using the `write()` function.

The parent process writes the message to the first pipe using the `write()` function and reads the modified message from the second pipe using the `read()` function. The modified message is then printed to the console.

## Usage

To build and run the code, use the following commands:

```bash
g++ -o pipe_example pipe_example.cpp
./pipe_example
```

## Notes

- The size of the buffer used to read from and write to the pipes is limited to 100 bytes. Make sure that the message does not exceed this size.
- The `strncat()` function is used to concatenate the messages in the child process. This function copies at most `BUF_SIZE - strlen(dest) - 1` characters from `src` to `dest`, where `BUF_SIZE` is the size of the buffer and `strlen(dest)` is the length of the string in `dest`. This ensures that the buffer does not overflow.