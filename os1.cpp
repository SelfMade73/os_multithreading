#include <iostream>
#include <thread>
#include <chrono>

#include <pthread.h>

struct Args{
    explicit Args( char ch ) : ch_(ch){};
    bool endFlag = false;
    char ch_;
};

void* function1( void* args )
{
    if (!args){
       pthread_exit( (void*)-1);
    }

    while ( !static_cast<Args*>(args)->endFlag ){
        (std::cout << static_cast<Args*>(args)->ch_ ).flush();
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    pthread_exit( (void*) 404 );
}

void* function2( void* args )
{
    if (!args){
        pthread_exit( (void*)-2);
    }

    while ( !static_cast<Args*>(args)->endFlag ){
        (std::cout << static_cast<Args*>(args)->ch_ ).flush();
        std::this_thread::sleep_for( std::chrono::milliseconds(500) );
    }
    pthread_exit( (void*) 403 );
}


int main()
{
    Args args1('1'), args2('2');
    int32_t result1, result2;
    pthread_t thread1, thread2;

    std::cout << "To stop threads press enter" << std::endl;

    pthread_create(&thread1,nullptr, function1, &args1 );
    pthread_create(&thread2,nullptr, function2, &args2 );

    std::cin.get();
    
    args1.endFlag = true;
    args2.endFlag = true;

    std::cout << "Joining threads..." << std::endl;
    pthread_join(thread1, (void**)&result1 );
    pthread_join(thread2, (void**)&result2 );

    std::cout << "Thread 1 finished with status-code: " << result1 << std::endl;
    std::cout << "Thread 2 finished with status-code: " << result2 << std::endl;

    return 0;
}
