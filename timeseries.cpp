// 20593794

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include "timeseries.h"
#include <string>

    // constructor
    TimeSeries::TimeSeries(const char* CSVfileName){
        this->fileName = CSVfileName;
        initVecFromCsv(fileName);
    }
    void TimeSeries::initVecFromCsv(const char* CSVfileName) {
        std::ifstream csv;
        std::stringstream str;
        csv.open(CSVfileName);
        bool isFeaturesLine = true;
        std::string line;
        std::string value;
        std::vector<std::string> features;
        std::vector<float> secLine;
        std::vector<std::vector<float>> csvMatrix;
        while (getline(csv, line)) {
            std::stringstream str(line);
            if (isFeaturesLine) {
                while (getline(str, value, ',')) {
                    features.push_back(value);
                }
                this->features = features;
                isFeaturesLine = false;
            } else {
                while (getline(str, value, ',')) {
                    secLine.push_back(stof(value));
                }
                csvMatrix.push_back(secLine);
                secLine.clear();
            }
        }
        this->csvMatrix = csvMatrix;
        csv.close();
    }

std::vector<float> TimeSeries::getFeatureVector(int x) const {
    std::vector<float> v;
    for(int i = 0; i < csvMatrix.size(); i++) {
        v.push_back(csvMatrix[i][x]);
    }
    return v;
}

std::vector<std::vector<float>> TimeSeries::getMatrix() {
    return csvMatrix;
}

std::vector<std::string> TimeSeries::getFeatures() {
    return features;
}

std::vector<float> TimeSeries::getTimeSeries(int x) {
    return csvMatrix[x];
}

int TimeSeries::getFeatureNum(std::string featureName) const {
    for(int i = 0; i < features.size(); i++) {
        if (features[i] == featureName){
            return i;
        }
    }
    return -1;
}

float TimeSeries::getValue(std::string featureName, int numOfLine){
    int numOfFeature = getFeatureNum(featureName);
    std::vector<float> v = getFeatureVector(numOfFeature);
    return v[numOfLine];
}
