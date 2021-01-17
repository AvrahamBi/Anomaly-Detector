// 20593794

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"


class CLI {
	DefaultIO* dio;
	std::vector<Command*> commands;
	HybridAnomalyDetector hybridDetector;
public:

    CLI(DefaultIO* dio);
    void start();
    void printCommands();
    std::vector<Command*> getVecOfCommands();
	virtual ~CLI() = default;
};

#endif /* CLI_H_ */