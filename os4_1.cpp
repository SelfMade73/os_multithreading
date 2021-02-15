#include <iostream>
#include <thread>
#include <chrono>

#include <unistd.h>

int main(int argc, const char** argv) {

    std::cout << "CURRENT PROCESS PID: " << getpid() << std::endl;
    std::cout << "PARENT  PROCESS PID: " << getppid() << std::endl;

    if ( argc < 2){
        std::cout << "Not enougth params" << std::endl;
        return -1;
    }

    for (size_t i = 1; i < argc; ++i)
    {
        std::cout << argv[i] << std::endl;
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }

    return 0;
}