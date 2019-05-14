#ifndef MACHINELEARNING_ISBM_H
#define MACHINELEARNING_ISBM_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"


class ISBM : public InstanceSelection {
public:
    ISBM(float ret, Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

protected:

    std::vector<Relevance> ComputeRelevances(const std::vector<Instance> &data);

    std::vector<Instance>
    SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data);

    std::vector<Instance>
    FindBorders(const std::vector<Instance> &topRelevants, const std::vector<std::vector<Instance>> &categories);

    Instance ComputeBorderRelevant(const std::vector<Instance> &data, const Instance &representative);

    Instance FindRelevant(const std::vector<Instance> &relevants, int category);

    float ret;
    Metric *m;
    int featuresSize;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_ISBM_H
