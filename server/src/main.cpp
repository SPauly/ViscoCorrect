#include<iostream>
#include "Server.h"

int main(){
    std::cout<<"Starting Server..."<<std::endl;
    Your_Project::Server *server = Your_Project::CreateServer();
    server->Run();
    delete server;
    std::cout<<"Server stopped. Press ENTER to close"<<std::endl;
    std::cin.get();
    return 0;
}