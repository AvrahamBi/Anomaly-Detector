// 20593794

#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <vector>
#include <string>


class TimeSeries{
    std::vector<std::string> features;
    std::vector<std::vector<float>> csvMatrix;
    const char* fileName;
public:
    TimeSeries(const char* CSVfileName);
    virtual ~TimeSeries() = default;
    void initVecFromCsv(const char* CSVfileName);
    std::vector<float> getFeatureVector(int x) const;
    std::vector<std::vector<float>> getMatrix();
    std::vector<std::string> getFeatures();
    std::vector<float> getTimeSeries(int i);
    int getFeatureNum(std::string featureName) const;
    float getValue(std::string featureName, int numOfLine);

};

#endif
