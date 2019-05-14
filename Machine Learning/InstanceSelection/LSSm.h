#ifndef MACHINELEARNING_LSSM_H
#define MACHINELEARNING_LSSM_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class LSSm : public InstanceSelection {
public:
    explicit LSSm(Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

    std::map<std::string, Distances> GetDistances();

protected:

    int FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories);

    Metric *m;
    std::map<std::string, Distances> dist;
};

#endif //MACHINELEARNING_LSSM_H
