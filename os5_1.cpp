#include <iostream>
#include <fstream>
#include <semaphore.h>
#include <fcntl.h>

#include <thread>
#include <chrono>

const std::string FILENAME( "text.txt" ); 
const std::string SEMNAME( "/test_sem_1" ); 
std::ofstream file;
sem_t* semaphore;


void func( bool* endFlag )
{
    while ( *endFlag == false )
    {
        sem_wait(semaphore);
        for (size_t i = 0; i < 6; i++)
        {
            (file << 2).flush();
            (std::cout << 2).flush();
            std::this_thread::sleep_for( std::chrono::seconds(1) );
        }
        sem_post(semaphore);
        std::this_thread::sleep_for( std::chrono::seconds(2) );
    }
}


int main()
{
    bool endFlag = false;
    file.open(FILENAME, std::ios::app );
    semaphore = sem_open( SEMNAME.c_str(), O_CREAT, 0644, 1 );
    std::thread thread( func, &endFlag);

    std::cout << "File is " << FILENAME << "\tto stop press enter..." << std::endl;
    std::cin.get();
    
    endFlag = true;
    thread.join();
    file.close();
    sem_close(semaphore);
    sem_unlink(SEMNAME.c_str());

    return 0;
}