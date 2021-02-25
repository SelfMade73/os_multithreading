#include <iostream>
#include <thread>
#include <chrono>
#include <queue>
#include <mutex>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int clientSocket;
bool flag1 = false, flag2 = false, flag3 = false, flag4 = false;
std::thread getRequestThd, handleRequestThd, sendResponseThd;
std::queue<std::string> requests, responses;
std::mutex m_sock, m_req_queue, m_res_queue;

std::string processRequest( std::string&& request ){
    if ( request.find("get_info") != std::string::npos ){
        return std::string("superInfo");
    }else if( request.find("get_time") != std::string::npos ){
        return std::string("superTime");
    }else{
        return std::string("requestWithoutKeywords");
    }
}

void getRequest(){
    std::vector<char> buf;
    while( !flag2 ){
        buf.resize(1024);
        (std::cout << "get" ).flush();
        if ( m_sock.try_lock() ){
             (std::cout << "get1" ).flush();
            int32_t bytes_red = recv( clientSocket, buf.data() , 1024, 0 );
            m_sock.unlock();
            if ( bytes_red > 0){
                (std::cout << std::string ( buf.data(), bytes_red ) ).flush();
                std::lock_guard<std::mutex> quard( m_req_queue );
                requests.push( std::string ( buf.data(), bytes_red ) );
            }
        }
    }
}

void handleRequest(){
    std::string request;
    (std::cout << "handle" ).flush();
    while( !flag3 ){
        if ( m_req_queue.try_lock() && !requests.empty() ){
            (std::cout << "locked" ).flush();
                request = requests.front();
                requests.pop();
                m_req_queue.unlock();
                const std::string response = processRequest( std::move( request ));
                std::lock_guard<std::mutex> guard( m_res_queue );
                responses.push( response );
        }
    }
}

void sendResponse(){
    (std::cout << "send" ).flush();
    while( !flag4 ){
        if ( m_res_queue.try_lock() && !responses.empty() ){
            auto response = responses.front();
            responses.pop();
            m_res_queue.unlock();
            std::lock_guard<std::mutex> guard( m_sock );
            send( clientSocket, response.c_str(), response.size(), 0);
        }
    }
}

void waitClient( int* listenSocket ){
    while ( !flag1 )
    {
        //(std::cout << "waiting" ).flush();
        clientSocket = accept(*listenSocket, NULL, NULL );    
        if ( clientSocket >= 0 ){
            (std::cout << "accepted" ).flush();
            getRequestThd = std::thread(getRequest);
            handleRequestThd = std::thread(handleRequest);
            sendResponseThd = std::thread ( sendResponse );
            getRequestThd.join();
            handleRequestThd.join();
            sendResponseThd.join();
        }
        else{
            perror("accept");
            (std::cout << "f" ).flush();
        }
    }
}

int main(int argc, const char** argv) {
    int listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8081);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind(listenSocket, (struct sockaddr *)&addr, sizeof(addr));
    listen(listenSocket, 1);

    std::thread waitThread ( waitClient, &listenSocket );
    (std::cout << "Press enter to stop " << std::endl).flush();

    std::cin.get();
    flag1 = true;
    flag2 = true;
    flag3 = true;
    flag4 = true;

    waitThread.join();

    close ( clientSocket );
    close ( listenSocket );
    return 0;
}