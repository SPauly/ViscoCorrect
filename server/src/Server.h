#pragma once

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "Your_Project_protocol.grpc.pb.h"

#include <iostream>
#include <string>
#include <memory>


namespace Your_Project 
{
    class Server final : public YourProject::YourProjectServer::Service
    {
    public:
        Server() = default;
        ~Server() = default;

        void Run();

    protected:
        grpc::Status Ping(::grpc::ServerContext*, const ::YourProject::Id*, ::YourProject::Id*) override;

    private:
        std::string m_server_address = "0.0.0.0:50000";
        grpc::ServerBuilder mgrpc_builder;
        std::unique_ptr<grpc::Server> mgrpc_server;
    };
    
    Server *CreateServer();

};