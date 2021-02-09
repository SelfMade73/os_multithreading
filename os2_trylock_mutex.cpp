#include <iostream>
#include <thread>
#include <chrono>

#include <pthread.h>

pthread_mutex_t mtx;
void* function1( void* endFlag )
{
    if (!endFlag){
       pthread_exit( NULL );
    }
    while ( !*static_cast<bool*>(endFlag) ){
        if ( pthread_mutex_trylock( &mtx ) == 0 ){
            for (int32_t i = 0; i < 5; ++i){
                (std::cout << "1" ).flush();
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
            pthread_mutex_unlock( &mtx );
        }
        std::this_thread::sleep_for( std::chrono::seconds(2) );
    }
    pthread_exit( NULL );
}

void* function2( void* endFlag )
{
    if (!endFlag){
       pthread_exit( NULL );
    }
    while ( !*static_cast<bool*>(endFlag) ){
        if ( pthread_mutex_trylock( &mtx ) == 0 ){
            for (int32_t i = 0; i < 5; ++i){
                (std::cout << "2" ).flush();
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
            pthread_mutex_unlock( &mtx );
        }
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    pthread_exit( NULL );
}


int main()
{
    bool end1 = false, end2 = false;
    int32_t result1, result2;
    pthread_t thread1, thread2;
    
    pthread_mutex_init(&mtx, NULL);
    std::cout << "To stop threads press enter" << std::endl;

    pthread_create(&thread1,nullptr, function1, &end1);
    pthread_create(&thread2,nullptr, function2, &end2);

    std::cin.get();
    
    end1 = true;
    end2 = true;

    std::cout << "Joining threads..." << std::endl;
    pthread_join(thread1, NULL );
    pthread_join(thread2, NULL );

    pthread_mutex_destroy(&mtx);

    return 0;
}
