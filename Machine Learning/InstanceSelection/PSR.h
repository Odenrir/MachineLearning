#ifndef MACHINELEARNING_PSR_H
#define MACHINELEARNING_PSR_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class PSR : public InstanceSelection {
public:
    PSR(float ret, Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

protected:

    std::vector<Relevance> ComputeRelevances(const std::vector<Instance> &data);

    std::vector<Instance>
    SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data);

    std::vector<Instance>
    FindBorders(const std::vector<Instance> &relevants, const std::vector<std::vector<Instance>> &complement);

    float ret;
    Metric *m;
    std::map<std::string, Distances> dist;
};

#endif //MACHINELEARNING_PSR_H
