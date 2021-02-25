#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <thread>
#include <chrono>
#include <sstream>


const std::string pipeName ("pipe");
int32_t fd_write;

std::string getInfo(){
    std::stringstream ss;
    utsname buf;
    uname( &buf );
    ss << buf.sysname << std::endl << buf.nodename << std::endl 
    << buf.release << std::endl << buf.version << std::endl << buf.machine << std::endl;
    return ss.str();
}

void func ( bool* endFlag )
{
    while (!*endFlag)
    {
        const std::string info = getInfo();
        std::cout << " INFO ::  " << info;
        write(fd_write,info.c_str(), info.length() );
        std::this_thread::sleep_for( std::chrono::seconds(3) );
    }
    
}


int main(int argc, char const *argv[])
{
    bool endFlag = false;
    mkfifo(pipeName.c_str(), 0644);
    fd_write = open(pipeName.c_str(), O_WRONLY);
    std::thread thrd ( func, &endFlag );

    std::cout << "To stop press enter..." << std::endl << std::endl << std::endl;
    std::cin.get();

    endFlag = true;
    thrd.join();
    close(fd_write);
    unlink(pipeName.c_str());

    return 0;
}
