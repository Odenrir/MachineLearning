#ifndef MACHINELEARNING_LSBO_H
#define MACHINELEARNING_LSBO_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "LSSm.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class LSBo : public InstanceSelection {
public:
    explicit LSBo(Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

    std::map<std::string, Distances> GetDistances();

protected:

    int FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories);

    Metric *m;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_LSBO_H
