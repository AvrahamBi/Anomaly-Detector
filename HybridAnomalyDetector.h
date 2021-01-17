// 20593794

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector: public SimpleAnomalyDetector {

public:
	HybridAnomalyDetector();
	virtual ~HybridAnomalyDetector() = default;
    bool isCorrelated(float correlation) override;
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
