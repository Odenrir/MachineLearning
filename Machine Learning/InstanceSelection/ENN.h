/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_ENN_H
#define MACHINELEARNING_ENN_H

#include "InstanceSelection.h"
#include "Metrics/Metric.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class ENN : public InstanceSelection {
public:
    ENN(Metric &m, int k);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

    std::map<std::string, Distances> GetDistances();

protected:

    std::vector<Distances> ComputeDistances(const std::vector<Instance> &data, const Instance &inst);

    int Classify(const Instance &inst, const std::vector<Instance> &train);

    std::vector<Instance> NearestInstances(const std::vector<Instance> &data, const Instance &inst);

    int k, labels;
    Metric *m;
    std::map<std::string, Distances> dist;
    std::map<int, int> idIndex;
    std::vector<int> vClasses;
};


#endif //MACHINELEARNING_ENN_H
