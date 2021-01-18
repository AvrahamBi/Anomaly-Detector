// 20593794

#ifndef SERVER_H_
#define SERVER_H_

#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include "commands.h"
#include "CLI.h"

class ClientHandler{
    public:
    virtual void handle(int clientID) = 0;
};

class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID);
};

class Server {
    int fd;
    bool isRunning;
	std::thread* t;
    sockaddr_in server;
    sockaddr_in client;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
