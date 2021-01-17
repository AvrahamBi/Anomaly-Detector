// 20593794

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string>
#include <math.h>
#include "minCircle.h"
#include <bits/stdc++.h>


struct correlatedFeatures{
    correlatedFeatures();
    std::string feature1,feature2;  // names of the correlated features
	float corrlation;
	Line lin_reg;
	bool isStrongCorrelation;
	Circle minimumCircle;
	float threshold;
};


class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
protected:
    std::vector<correlatedFeatures> cf;
    float correlationForDetect;
    std::string trainFile, testFile;
    // map of anomalies reports
    std::vector<std::pair<float, std::string>> anomaliesReport;
    int numberOfLines;

public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector() = default;
	virtual void learnNormal(const TimeSeries& ts);
	virtual std::vector<AnomalyReport> detect(const TimeSeries& ts);
    std::vector<correlatedFeatures> getNormalModel(){return cf;}
	float findThreshold(int size, Point** points);
    virtual bool isCorrelated(float correlation);
    float getDistance(Point p1, Point p2);
    std::string getTrainFile() {return trainFile;}
    std::string getTestFile() {return testFile;}
    void setTrainFile(std::string s) {trainFile = s;}
    void setTestFile(std::string s) {testFile = s;}
    float getCorrelation() {return correlationForDetect;}
    void setCorrelation(float f) {correlationForDetect = f;}
    std::vector<std::pair<float, std::string>> getAnomaliesReport() {return anomaliesReport;}
    int getNumberOfLines(){return numberOfLines;}
};



#endif
