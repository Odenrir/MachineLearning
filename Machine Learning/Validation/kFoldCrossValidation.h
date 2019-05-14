#pragma once

#include "Validation.h"
#include "InstanceSelection/InstanceSelection.h"
#include "Misc/Utils.h"

class kFoldCrossValidation : public Validation {
public:
    kFoldCrossValidation();

    kFoldCrossValidation(int k, const std::vector<Instance> &train, Classifier &c);

    void Init() override;

    double Validate() override;

    std::vector<float> ValidateIS(InstanceSelection &algorithm, std::string fileName = "result.txt");

protected:
    void Split();

    void SplitStratified();

    void
    WriteFile(const std::string &fileName, std::vector<float> acc, std::vector<float> accIS, std::vector<float> ret,
              std::vector<long> runtimes);

    std::vector<Instance> train;
    std::vector<std::vector<Instance>> trainFolds;
    std::vector<std::vector<Instance>> testFolds;
    int k;
};