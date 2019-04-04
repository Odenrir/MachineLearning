#pragma once

#include "Instance.h"

class Classifier {
public:
    virtual void Init() = 0;

    virtual void Learn(const std::vector<Instance> &train) = 0;

    virtual int Classify(Instance &inst) = 0;

    virtual const std::vector<int> Classify(std::vector<Instance> &test) = 0;

};