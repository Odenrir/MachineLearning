/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
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

    void Clear() override;

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
