// 20593794

#include "HybridAnomalyDetector.h"

HybridAnomalyDetector::HybridAnomalyDetector() : SimpleAnomalyDetector(){

}


bool HybridAnomalyDetector::isCorrelated(float correlation) {
    return 0.5 <= correlation;
}


