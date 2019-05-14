#ifndef MACHINELEARNING_ISRB_H
#define MACHINELEARNING_ISRB_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class ISRB : public InstanceSelection {
public:
    ISRB(float ret, Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

protected:

    std::vector<Relevance> ComputeRelevances(const std::vector<Instance> &data);

    std::vector<Instance>
    SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data);

    std::vector<Instance>
    FindBorders(const std::vector<Instance> &topRelevants, const std::vector<std::vector<Instance>> &categories);

    Instance ComputeCentroid(const std::vector<Instance> &data);

    Instance FindRelevant(const std::vector<Instance> &relevants, int category);

    float ret;
    Metric *m;
    int featuresSize;
    std::map<std::string, Distances> dist;
};

#endif //MACHINELEARNING_ISRB_H
