/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_ISRT_H
#define MACHINELEARNING_ISRT_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class ISRT : public InstanceSelection {
public:
    ISRT(float t, Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;


protected:

    std::vector<Relevance> ComputeRelevances(const std::vector<Instance> &data);

    Instance
    SelectRelevant(std::vector<Relevance> &relevances, const std::vector<Instance> &data);

    std::vector<Instance>
    FindBorders(const Instance &relevant, const std::vector<Instance> &category);

    float t;
    Metric *m;
    std::map<std::string, Distances> dist;
    std::map<int, double> radius;
};


#endif //MACHINELEARNING_ISRT_H
