#pragma once

#include "Metric.h"

class Euclidean : public Metric {
public:
    Euclidean();

    void Init() override;

    double Distance(const Instance &inst1, const Instance &inst2) override;

    void Normalize(const std::vector<Instance> &train);

protected:
    std::vector<double> ranges;
};