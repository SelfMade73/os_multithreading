#include <iostream>
#include <thread>
#include <chrono>

#include <pthread.h>


void* function1( void* endFlag )
{
    if (!endFlag){
       pthread_exit( (void*)-1);
    }

    while ( !*static_cast<bool*>(endFlag) ){
        (std::cout << "1").flush();
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    pthread_exit( (void*) 404 );
}

void* function2( void* endFlag )
{
    if (!endFlag){
        pthread_exit( (void*)-2);
    }

    while ( !*static_cast<bool*>(endFlag) ){
        (std::cout << "2").flush();
        std::this_thread::sleep_for( std::chrono::milliseconds(500) );
    }
    pthread_exit( (void*) 403 );
}


int main()
{
    bool end1 = false, end2 = false;
    int32_t result1, result2;
    pthread_t thread1, thread2;

    std::cout << "To stop threads press enter" << std::endl;

    pthread_create(&thread1,nullptr, function1, &end1);
    pthread_create(&thread2,nullptr, function2, &end2);

    std::cin.get();
    
    end1 = true;
    end2 = true;

    std::cout << "Joining threads..." << std::endl;
    pthread_join(thread1, (void**)&result1 );
    pthread_join(thread2, (void**)&result2 );

    std::cout << "Thread 1 finished with status-code: " << result1 << std::endl;
    std::cout << "Thread 2 finished with status-code: " << result2 << std::endl;

    return 0;
}
