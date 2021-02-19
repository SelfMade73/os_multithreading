#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/mman.h>
#include <string.h>

#include <thread>
#include <chrono>
#include <sstream>


sem_t* w_sem, *r_sem;
int32_t fd;
char *ptr;
const int32_t buffSize = 1024;
const std::string writeSemName("/write_sem");
const std::string readSemName("/read_sem");
const std::string memoryName("shared_memory");

void func ( bool* endFlag )
{
    while (!*endFlag)
    {
        sem_wait(w_sem);
        ptr = (char*) mmap(0, buffSize, PROT_READ, MAP_SHARED, fd, 0);
        std::cout << std::string( ptr, buffSize );
        sem_post(r_sem);
        std::this_thread::sleep_for( std::chrono::seconds(3) );
    }
    
}

int main(int argc, const char** argv) 
{
    bool endFlag = false;
    fd = shm_open( memoryName.c_str(), O_RDWR | O_CREAT, 0644 );

    w_sem = sem_open( writeSemName.c_str(), O_CREAT, 0644, 2 );
    r_sem = sem_open( readSemName.c_str(), O_CREAT, 0644, 2 );

    std::thread thrd ( func, &endFlag );

    std::cout << "To stop press enter..." << std::endl << std::endl << std::endl;
    std::cin.get();
    endFlag = true;

    thrd.join();
    sem_close(w_sem);
    sem_unlink(writeSemName.c_str());
    sem_close(r_sem);
    sem_unlink(readSemName.c_str());
    close( fd );
    shm_unlink( memoryName.c_str() );
    return 0;
}