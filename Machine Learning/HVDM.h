#ifndef MACHINELEARNING_HVDM_H
#define MACHINELEARNING_HVDM_H

#include "Metric.h"
#include <map>

class HVDM : public Metric {
public:
    HVDM();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &train);

protected:
    double VDM(int index, int x, int y);

    double normalizedDiff(int index, float x, float y);

    void ComputeOccurrences(const std::vector<Instance> &train);

    void ComputeStandardDeviation(const std::vector<Instance> &train);

    std::vector<double> stdDev4;
    std::map<int, std::map<int, std::map<int, int>>> naC;
    std::map<int, std::map<int, int>> na;
    int labels;
};


#endif //MACHINELEARNING_HVDM_H
