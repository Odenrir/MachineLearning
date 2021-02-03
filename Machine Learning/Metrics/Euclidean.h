/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#pragma once
#include "Metric.h"

class Euclidean : public Metric {
public:
    Euclidean();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &dataset);

protected:
    std::vector<double> ranges;
};