#pragma once

#include "Validation.h"
#include "Utils.h"

class KFoldCrossValidation : public Validation {
public:
    KFoldCrossValidation();

    KFoldCrossValidation(int k, const std::vector<Instance> &train, Classifier &c);

    void Init() override;

    double Validate() override;

protected:
    void Split();
    void SplitStratified();

    std::vector<Instance> train;
    std::vector<std::vector<Instance>> trainFolds;
    std::vector<std::vector<Instance>> testFolds;
    int k;
};