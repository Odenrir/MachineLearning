/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
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

    std::vector<Instance> GetIntersectionRepresentatives();

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
    std::vector<Instance> iRrepresentatives;
};

#endif //MACHINELEARNING_ISRB_H
