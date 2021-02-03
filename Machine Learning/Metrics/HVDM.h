/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_HVDM_H
#define MACHINELEARNING_HVDM_H

#include "Metric.h"
#include <map>

class HVDM : public Metric {
public:
    HVDM();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &dataset);

protected:
    double VDM(int index, int x, int y);

    double normalizedDiff(int index, float x, float y);

    void ComputeOccurrences(const std::vector<Instance> &dataset);

    void ComputeStandardDeviation(const std::vector<Instance> &dataset);

    std::vector<double> stdDev4;
    std::map<int, std::map<int, std::map<int, int>>> naC;
    std::map<int, std::map<int, int>> na;
    int labels;
};


#endif //MACHINELEARNING_HVDM_H
