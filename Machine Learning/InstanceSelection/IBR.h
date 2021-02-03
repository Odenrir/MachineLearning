/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_IBR_H
#define MACHINELEARNING_IBR_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "ENN.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class IBR : public InstanceSelection {
public:
    IBR(int k, float hiR, float meR, float loR, Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

protected:

    std::vector<Relevance>
    ComputeRanks(const std::vector<Instance> &category, const std::vector<std::vector<Instance>> &categories);

    double ComputeRank(const Instance &reference, const std::vector<std::vector<Instance>> &categories);

    std::vector<int> SelectInstances(std::vector<Relevance> &ranks, const std::vector<Instance> &data);

    Distances Similarity(const Instance &x, const Instance &y);

    Metric *m;
    int k;
    float hiR, meR, loR;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_IBR_H
