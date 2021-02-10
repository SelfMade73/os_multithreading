#include <iostream>
#include <thread>
#include <chrono>

#include <pthread.h>

struct Args
{
    explicit Args( pthread_mutex_t* mtx ) : mtx_( mtx ){}
    bool endFlag = false;
    pthread_mutex_t* mtx_;
};



void* function1( void* args )
{
    if (!args){
       pthread_exit( NULL );
    }

    Args* args_ = static_cast<Args*>(args);

    while ( args_->endFlag == false ){
        pthread_mutex_lock( (args_->mtx_ ));
            for (int32_t i = 0; i < 5; ++i){
                (std::cout << "1" ).flush();
                std::this_thread::sleep_for( std::chrono::milliseconds( 200 ) );
            }
        pthread_mutex_unlock( (args_->mtx_ ) );
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    pthread_exit( NULL );
}

void* function2( void* args )
{
    if (!args){
       pthread_exit( NULL );
    }

    Args* args_ = static_cast<Args*>(args);

    while ( args_->endFlag == false ){
        pthread_mutex_lock( (args_->mtx_ ) );
            for (int32_t i = 0; i < 5; ++i){
                (std::cout << "2" ).flush();
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
            }
        pthread_mutex_unlock( (args_->mtx_ ) );
        std::this_thread::sleep_for( std::chrono::seconds(1) );
    }
    pthread_exit( NULL );
}


int main()
{
    pthread_mutex_t mtx;
    int32_t result1, result2;
    pthread_t thread1, thread2;
    
    pthread_mutex_init(&mtx, NULL);
    Args args1(&mtx) ,args2(&mtx);
    std::cout << "To stop threads press enter" << std::endl;

    pthread_create(&thread1,nullptr, function1, &args1);
    pthread_create(&thread2,nullptr, function2, &args2);

    std::cin.get();
    
    args1.endFlag = true;
    args2.endFlag = true;

    std::cout << "Joining threads..." << std::endl;
    pthread_join(thread1, NULL );
    pthread_join(thread2, NULL );

    pthread_mutex_destroy(&mtx);

    return 0;
}
