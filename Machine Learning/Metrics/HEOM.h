/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_HEOM_H
#define MACHINELEARNING_HEOM_H

#include "Metric.h"

class HEOM : public Metric {
public:
    HEOM();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &dataset);

protected:
    std::vector<double> ranges;

};


#endif //MACHINELEARNING_HEOM_H
