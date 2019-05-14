#ifndef MACHINELEARNING_PSC_H
#define MACHINELEARNING_PSC_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Clustering/kMeans.h"
#include "Clustering/PAM.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class PSC : public InstanceSelection {
public:
    PSC(int c, Metric &m);

    void Init() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

protected:

    bool IsHomogeneous(const std::vector<Instance> &data);

    std::vector<Instance>
    FindBorders(const std::vector<Instance> &data);

    int c, t;
    Metric *m;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_PSC_H
