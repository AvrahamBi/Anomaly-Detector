// 20593794

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"
#include <bits/stdc++.h>
#include <algorithm>

class DefaultIO{
public:
	virtual std::string read()=0;
	virtual void write(std::string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO() = default;
};

// doesnt matter for MainTrain because
// MainTrain is talking with CLI with a unique IO
/*class StandartIO : public DefaultIO {
public:
    std::string read() {
        std::string s;
        std::cin >> s;
        return s;
    }
    void read(float* f) {
        std::cin >> *f;
    }
    void write(std::string text) {
        std::cout << text;
    }
    void write(float f) {
        std::cout << f;
    }
};*/


/*class SocketIO: public DefaultIO {

};*/


class Command{
protected:
	DefaultIO* dio;
	std::string description;
	HybridAnomalyDetector* hybridDetector;

public:
	Command(DefaultIO* dio, HybridAnomalyDetector* h):dio(dio){
	    hybridDetector = h;
	}
	virtual void execute()=0;
	virtual std::string getDescription(){return description;};
	virtual ~Command() = default;
};



class Command1UploadTimeSeries: public Command {
public:
    Command1UploadTimeSeries(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
        this->description = "1.upload a time series csv file\n";
        hybridDetector = h;

    }
    virtual ~Command1UploadTimeSeries() = default;


    void execute(){
        // initialize names of files
        std::string clientTrainFile = "anomalyTrain.csv";
        std::string clientTestFile = "anomalyTest.csv";
        // set the files name in hybrid detector
        hybridDetector->setTrainFile(clientTrainFile);
        hybridDetector->setTestFile(clientTestFile);

        std::ofstream trainFile, testFile;
        trainFile.open(clientTrainFile);
        testFile.open(clientTestFile);
        dio->write("Please upload your local train CSV file.\n");
        std::string line;
        while (true) {
            line = dio->read();
            if (!line.compare("done")) {break;}
            trainFile << line << "\n";
        }
        dio->write("Upload complete.\n");

        dio->write("Please upload your local test CSV file.\n");
        while (true) {
            line = dio->read();
            if (!line.compare("done")) {break;}
            testFile << line << "\n";
        }
        dio->write("Upload complete.\n");

        testFile.close();
        trainFile.close();
    }
};



class Command2AlgorithmSettings: public Command {
public:
    Command2AlgorithmSettings(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
            this->description = "2.algorithm settings\n";
    }
    virtual ~Command2AlgorithmSettings()= default;

    void execute(){
        float currentCorrelation = hybridDetector->getCorrelation();
        float newCorrelation;

        dio->write("The current correlation threshold is ");
        dio->write(currentCorrelation);
        dio->write("\n");
        dio->write("Type a new threshold\n");
        while(true) {
            newCorrelation = std::stof(dio->read());
            if ( 0 < newCorrelation && newCorrelation < 1) {
                hybridDetector->setCorrelation(newCorrelation);
                break;
            }
            dio->write("please choose a value between 0 and 1.\n");
        }
    }
};

class Command3DetectAnomalies: public Command {
public:
    Command3DetectAnomalies(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
            this->description = "3.detect anomalies\n";
    }
    virtual ~Command3DetectAnomalies()= default;

    void execute(){
        std::string testFileName = hybridDetector->getTrainFile();
        std::string trainFileName = hybridDetector->getTestFile();
        const char* clientTrainFile = testFileName.data();
        const char* clientTestFile = trainFileName.data();

        TimeSeries train(clientTrainFile);
        TimeSeries test(clientTestFile);
        hybridDetector->learnNormal(train);

        // i dont need this vector but the function isnt void
        std::vector<AnomalyReport> a = hybridDetector->detect(test);
        dio->write("anomaly detection complete.\n");
    }
};

class Command4DisplayResults: public Command {
public:
    Command4DisplayResults(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
            this->description = "4.display results\n";
    }
    virtual ~Command4DisplayResults()= default;

    void execute(){
        std::vector<std::pair<float, std::string>> anomalies = hybridDetector->getAnomaliesReport();
        for(int i = 0; i < anomalies.size(); i++) {
            dio->write(anomalies[i].first);
            dio->write("\t");
            dio->write(anomalies[i].second);
            dio->write("\n");
        }
        dio->write("Done.");
        dio->write("\n");
    }


};

class Command5UploadAnomalies: public Command {
public:
    Command5UploadAnomalies(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
            this->description = "5.upload anomalies and analyze results\n";

    }
    virtual ~Command5UploadAnomalies()= default;

    void execute(){
        std::string line, subStr;
        dio->write("Please upload your local anomalies file.\n");
        std::vector<std::pair<float, float>> clientAnomalies;
        std::vector<float> detectAnomaliesNoDescrip;    // vector of anomalies without the description

        for(int i = 0; i < hybridDetector->getAnomaliesReport().size(); i++) {
            detectAnomaliesNoDescrip.push_back(hybridDetector->getAnomaliesReport()[i].first);
        }
        // get the anomalies from user
        float f1 = 0, f2 = 0;
        while(true) {
            line = dio->read();
            if (!line.compare("done")) {
                break;}
            std::stringstream ss(line);
            getline(ss, subStr, ',');
            f1 = stof(subStr);
            getline(ss, subStr, ',');
            f2 = stof(subStr);
            clientAnomalies.push_back(std::make_pair(f1, f2));
        }
        dio->write("Upload complete.\n");

        float p = clientAnomalies.size();
        float n = hybridDetector->getNumberOfLines(); // num of lines
        float negative = n;
        for(int i = 0; i < p; i++) {
           negative -= (clientAnomalies[i].second - clientAnomalies[i].first + 1);   // plus 1 because its include the first and the last (we have 3 lines in a range of '1 - 2')
        }
        float tp = 0;
        bool backToFirstLoop = false;
        // iterate over the lines of the client file
        for(int i = 0; i < p; i++) {
            backToFirstLoop = false;
            // iterate over the detector anomalies according to the range of the specific line
            for(int j = clientAnomalies[i].first; j <= clientAnomalies[i].second; j++) {
                // iterate over the detector anomalies
                for(int k = 0; k < detectAnomaliesNoDescrip.size(); k++) {
                    if (j == detectAnomaliesNoDescrip[k]) {
                        tp++;
                        backToFirstLoop = true;
                        break;
                    }
                }
                if(backToFirstLoop){break;}
            }
        }

        std::string temp, nextDesc;
        std::vector<std::pair<float, std::string>> reports = hybridDetector->getAnomaliesReport();
        std::vector<std::pair<float, float>> anomalyRange;
        int size = reports.size();
        anomalyRange.push_back(std::make_pair(reports[0].first, reports[0].first));
        for(int i = 1; i < size; i++) {
            temp = reports[i].second;
            if (temp != reports[i + 1].second){
                anomalyRange.back().second = reports[i].first;
                if (i < size - 1) {
                    anomalyRange.emplace_back(std::make_pair(reports[i + 1].first, reports[i + 1].first));
                }
            }
        }
        float fp = 0;
        int miss;
        // iterate over our ranges
        for(int i = 0; i < anomalyRange.size(); i++) {
            miss = 0;
            // iterate over client's ranges
            for(int j = 0; j < clientAnomalies.size(); j++) {
                float detectorBegin = anomalyRange[i].first;
                float detectorEnd = anomalyRange[i].second;
                float clientBegin = clientAnomalies[j].first;
                float clientEnd = clientAnomalies[j].second;
                // if not in a range
                if (detectorEnd < clientBegin || clientEnd < detectorBegin ) {
                    miss++;
                }
            }
            // if it ןs a miss in all of the client ranges
            if (miss == clientAnomalies.size()) {
                fp++;
            }
        }

        float truePositiveRate = tp / p;
        float falsePositiveRate = fp / negative;
        truePositiveRate = std::floor(truePositiveRate * 1000) / 1000;
        falsePositiveRate = std::floor(falsePositiveRate * 1000) / 1000;

        dio->write("True Positive Rate: ");
        dio->write(truePositiveRate);
        dio->write("\n");

        dio->write("False Positive Rate: ");
        dio->write(falsePositiveRate);
        dio->write("\n");
    }
};



class Command6Exit: public Command {
public:
    Command6Exit(DefaultIO* dio, HybridAnomalyDetector* h):Command(dio, h) {
            this->description = "6.exit\n";

    }
    virtual ~Command6Exit()= default;

    void execute(){
        // no implementaion for this function
    }
};


#endif /* COMMANDS_H_ */
