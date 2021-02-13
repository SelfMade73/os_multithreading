#include <sys/types.h>
#include <sys/wait.h> 
#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>


int main(int argc, const char** argv) {
    pid_t pid = fork();
    if ( pid == 0 ){

#pragma GCC diagnostic ignored "-Wwrite-strings"
        char *const params[] = { "/home/aleksey/Desktop/os4", "Hello", "World", NULL}; 
#pragma GCC diagnostic pop

        execv(params[0],params);
    }else{
        int32_t status;
        while( waitpid(pid, &status, WNOHANG) == 0 ){
            std::this_thread::sleep_for( std::chrono::milliseconds(500));
        }

        std::cout << std::endl << std::endl <<  "PARENT PARENT PID " << getppid() << std::endl <<
                                                "PARENT PID " << getpid() << std::endl <<
                                                "CHILD PID " << pid << std::endl;
        std::cout << "Exit code: " << status << std::endl;
    }

    return 0;
}