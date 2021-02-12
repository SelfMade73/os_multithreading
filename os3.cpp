#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>

#include <pthread.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <fcntl.h>  

std::string getInfo(){
    std::stringstream ss;
    utsname buf;
    uname( &buf );
    ss << buf.sysname << std::endl << buf.nodename << std::endl 
    << buf.release << std::endl << buf.version << std::endl << buf.machine << std::endl;
    return ss.str();
}

int fileDesc[2];

void* writeFunction( void* endFlag )
{
    if (!endFlag){
       pthread_exit( NULL );
    }
    std::string buffer;
    while ( !*static_cast<bool*>(endFlag) ){
        buffer =  getInfo();
        write(fileDesc[1],buffer.c_str(), buffer.size());
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    pthread_exit( NULL );
}

void* readFunction( void* endFlag )
{
    if (!endFlag){
       pthread_exit( NULL );
    }
    std::vector<char> buffer;
    while ( !*static_cast<bool*>(endFlag) ){
        buffer.resize(1024);
        int32_t result =  read(fileDesc[0],buffer.data(), buffer.size());
        if ( result == -1){
            (std::cout << "Errno code is : " << errno << std::endl).flush();
        }else if ( result == 0 ){
            (std::cout << "Data is empty " << std::endl).flush();
        }else{
            buffer.shrink_to_fit();
            std::cout << "Message from writeFunction: " << std::endl <<  std::string(buffer.data(), result)  << std::endl;    
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    pthread_exit( NULL );
}


int main()
{
    bool end1 = false, end2 = false;
    int32_t result1, result2;
    pthread_t thread1, thread2;
    
    pipe(fileDesc);
    fcntl( fileDesc[0], F_SETFL, fcntl(fileDesc[0], F_GETFL) | O_NONBLOCK);

    std::cout << "To stop threads press enter" << std::endl;

    pthread_create(&thread1,nullptr, writeFunction, &end1);
    pthread_create(&thread2,nullptr, readFunction, &end2);

    std::cin.get();
    
    end1 = true;
    end2 = true;

    std::cout << "Joining threads..." << std::endl;
    pthread_join(thread1, NULL );
    pthread_join(thread2, NULL );

    close(fileDesc[0]);
    close(fileDesc[1]);

    return 0;
}
