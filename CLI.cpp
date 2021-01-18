// 20593794

#include "CLI.h"

//
#include <iostream>
#include <string>
#include <sstream>
//
CLI::CLI(DefaultIO* dio) {
    HybridAnomalyDetector hybrid;
    hybridDetector = hybrid;
    this->dio = dio;
    commands.push_back(new Command1UploadTimeSeries(dio, &hybridDetector));
    commands.push_back(new Command2AlgorithmSettings(dio, &hybridDetector));
    commands.push_back(new Command3DetectAnomalies(dio, &hybridDetector));
    commands.push_back(new Command4DisplayResults(dio, &hybridDetector));
    commands.push_back(new Command5UploadAnomalies(dio, &hybridDetector));
    commands.push_back(new Command6Exit(dio, &hybridDetector));
}

void CLI::printCommands() {
    for(int i = 0; i < commands.size(); i++) {
        dio->write(commands[i]->getDescription());
    }
}

void CLI::start(){
    std::string str;
    int chosenNum = 0;
    std::string s;
    while(true) {

        dio->write("Welcome to the Anomaly Detection Server.\n");
        dio->write("Please choose an option:\n");
        printCommands();

        s = dio->read();
        std::stringstream num(s);
        num >> chosenNum;

        if (chosenNum == 1) {
            commands[0]->execute();
            continue;
        }
        if (chosenNum == 2) {
            commands[1]->execute();
            continue;
        }
        if (chosenNum == 3) {
            commands[2]->execute();
            continue;
        }
        if (chosenNum == 4) {
            commands[3]->execute();
            continue;
        }
        if (chosenNum == 5) {
            commands[4]->execute();
            continue;
        }
        if (chosenNum == 6) {
            for(int i = 0; i < commands.size(); i++) {
                delete commands[i];
            }
            break;
        }
    }
}

std::vector<Command*> CLI::getVecOfCommands() {
    return commands;
}

