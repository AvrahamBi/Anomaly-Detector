// 20593794

#include "Server.h"

Server::Server(int port)throw (const char*) {
    isRunning = false;
    fd = socket(AF_INET,SOCK_STREAM,0);
    if (fd < 0) {
        throw "Socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if(bind(fd,(struct sockaddr*) &server, sizeof(server)) < 0) {
        throw "Bind failed";
    }
    if(listen(fd, 3) < 0) {
        throw "Listen failed";
    }
}

void Server::start(ClientHandler& ch)throw(const char*){
    isRunning = true;
    t = new std::thread([&ch,this]() {
        while(isRunning) {
            socklen_t clientSize = sizeof(client);
            alarm(1);
            int acceptedClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            alarm(0);
            if (acceptedClient < 0) {
                throw "accept failed";
            }
            ch.handle(acceptedClient);
            close(acceptedClient);
        }
        close(fd);
    });
}

void Server::stop(){
    isRunning = false;
	t->join();
}

Server::~Server() {
}

void AnomalyDetectionHandler::handle(int clientID) {
    SocketIO dio(clientID);
    CLI ClientHandler(&dio);
    ClientHandler.start();
}