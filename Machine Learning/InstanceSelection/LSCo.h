/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_LSCO_H
#define MACHINELEARNING_LSCO_H

#include "Metrics/Metric.h"
#include "InstanceSelection.h"
#include "LSSm.h"
#include "Misc/Utils.h"
#include "Misc/Types.h"

class LSCo : public InstanceSelection {
public:
    explicit LSCo(Metric &m);

    void Init() override;

    void Clear() override;

    std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) override;

    std::map<std::string, Distances> GetDistances();

protected:

    std::vector<int> LSClustering(std::vector<LocalSet> &ls);

    int FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories);

    Metric *m;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_LSCO_H
