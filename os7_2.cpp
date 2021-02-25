#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <thread>
#include <chrono>
#include <sstream>
#include <vector>


const std::string pipeName ("pipe");
int32_t fd_read;
int32_t buffSize = 1024;

void func ( bool* endFlag )
{
    std::vector<char> buffer;
    while (!*endFlag)
    {
        buffer.resize(buffSize);
        int32_t written = read(fd_read,buffer.data(), buffSize);
        std::cout << std::string( buffer.data(), written ) << std::endl;
        std::this_thread::sleep_for( std::chrono::seconds(3) );
    }
}


int main(int argc, char const *argv[])
{
    bool endFlag = false;
    mkfifo(pipeName.c_str(), 0644);
    fd_read = open(pipeName.c_str(), O_RDONLY);
    std::thread thrd ( func, &endFlag );

    std::cout << "To stop press enter..." << std::endl << std::endl << std::endl;
    std::cin.get();

    endFlag = true;
    thrd.join();
    close(fd_read);
    unlink(pipeName.c_str());

    return 0;
}
