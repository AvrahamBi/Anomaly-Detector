// 20593794

#include "SimpleAnomalyDetector.h"
#include <string>

SimpleAnomalyDetector::SimpleAnomalyDetector() {
    correlationForDetect = 0.9;
}


void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	TimeSeries timeSeries = ts;
	int numOfFeatures = timeSeries.getFeatures().size();
	int numOfLines = timeSeries.getFeatureVector(0).size();
	for(int i = 0; i < numOfFeatures; i++) {
	    correlatedFeatures currentCF;
	    currentCF.corrlation = 0;
        std::vector<float> featureVector1 = timeSeries.getFeatureVector(i);
        for(int j = i + 1; j < numOfFeatures; j++) {
            std::vector<float> featureVector2 = timeSeries.getFeatureVector(j);
            //
            float currentPearson = pearson(&featureVector1[0], &featureVector2[0], numOfLines);
            if((currentCF.corrlation < currentPearson) && (isCorrelated(currentPearson))){
                Point** points = new Point*[numOfLines];
                for(int x = 0; x < numOfLines; x++) {
                    points[x] = new Point(timeSeries.getFeatureVector(i)[x], timeSeries.getFeatureVector(j)[x]);
                }
                // if its a strong correlation features
                if (correlationForDetect <= currentPearson && 0.5 < currentPearson) {
                    currentCF.feature1 = timeSeries.getFeatures()[i];
                    currentCF.feature2 = timeSeries.getFeatures()[j];
                    currentCF.corrlation = currentPearson;
                    currentCF.lin_reg = linear_reg(points, numOfLines);
                    currentCF.isStrongCorrelation = true;
                    currentCF.threshold = findThreshold(numOfLines, points);

                // if its a weak correlation feature
                } else {
                    currentCF.feature1 = timeSeries.getFeatures()[i];
                    currentCF.feature2 = timeSeries.getFeatures()[j];
                    currentCF.corrlation = currentPearson;
                    currentCF.lin_reg = linear_reg(points, numOfLines);
                    currentCF.isStrongCorrelation = false;
                    minCircle m;
                    Circle c = m.findMinCircle(points, numOfLines);
                    currentCF.minimumCircle = c;
                    currentCF.threshold = currentCF.minimumCircle.radius * 1.1;
                }
                for(int i = 0; i < numOfLines; i++) {
                    delete points[i];
                }
                delete[] points;
            }
        }
        // if its a correlation
        if (isCorrelated(currentCF.corrlation)) {cf.push_back(currentCF);}
	}
}

std::vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
    std::vector<AnomalyReport> reports;
    // iterate over the cf
    for(int i = 0; i < cf.size(); i++) {
        int numOfFeature1 = ts.getFeatureNum(cf[i].feature1);
        int numOfFeature2 = ts.getFeatureNum(cf[i].feature2);
        std::vector<float> featureVector1 = ts.getFeatureVector(numOfFeature1);
        std::vector<float> featureVector2 = ts.getFeatureVector(numOfFeature2);
        numberOfLines = featureVector1.size();
        // if the correlated features has strong correlation
        if (cf[i].isStrongCorrelation) {
            // iterate over the two correlated vectors to find anomaly
            for (int j = 0; j < featureVector1.size(); j++) {
                Point p(featureVector1[j], featureVector2[j]);
                float deviation = dev(p, cf[i].lin_reg);
                // anomaly found
                if (cf[i].threshold < deviation) {
                    std::string description = cf[i].feature1 + "-" + cf[i].feature2;
                    AnomalyReport report(description, j + 1);
                    reports.push_back(report);
                }
            }
            // if feature have weak correlation
        } else {
            // iterate over the two correlated vectors to find anomaly
            for (int j = 0; j < featureVector1.size(); j++) {
                // point consist of the 2 values
                Point p(featureVector1[j], featureVector2[j]);
                Point centerOfCircle = cf[i].minimumCircle.center;
                float deviation = getDistance(p, centerOfCircle);
                // if anomaly found
                if (cf[i].threshold < deviation) {
                    std::string description = cf[i].feature1 + "-" + cf[i].feature2;
                    AnomalyReport report(description, j + 1);
                    reports.push_back(report);
                }
            }
        }
	}
    //anomaliesReport.clear();
    for(int i = 0; i < reports.size(); i++) {
        anomaliesReport.emplace_back(std::make_pair(reports[i].timeStep, reports[i].description));
    }
    return reports;
}

float SimpleAnomalyDetector::findThreshold(int size, Point** points){
    Line linearRegression = linear_reg(points, size);
    float finalDev = 0;
    float tempDev;
    for(int i = 0; i < size; i++) {
        Point p(points[i]->x, points[i]->y);
        tempDev = dev(p, linearRegression);
        finalDev = std::max(tempDev, finalDev);
    }
    return finalDev *= 1.1;  // TODO maybe should be 1.2 or 1.15
}

bool SimpleAnomalyDetector::isCorrelated(float correlation) {
    return correlationForDetect <= correlation;
}

float SimpleAnomalyDetector::getDistance(Point p1, Point p2) {
    return (float)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// correlated feature constructor
correlatedFeatures::correlatedFeatures() {
    minimumCircle = Circle(Point(), 0);
}
