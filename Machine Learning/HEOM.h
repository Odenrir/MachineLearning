#ifndef MACHINELEARNING_HEOM_H
#define MACHINELEARNING_HEOM_H
#include "Metric.h"

class HEOM: public Metric {
public:
    HEOM();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &train);

protected:
    std::vector<double> ranges;

};


#endif //MACHINELEARNING_HEOM_H
