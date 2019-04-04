#ifndef MACHINELEARNING_KNN_H
#define MACHINELEARNING_KNN_H

#endif //MACHINELEARNING_KNN_H

#include "Classifier.h"
#include "Metric.h"
#include "Utils.h"
#include "Types.h"

class KNN : public Classifier {
public:
    KNN(int k, Metric &m);

    void Init() override;

    void Learn(const std::vector<Instance> &train) override;

    int Classify(Instance &inst) override;

    const std::vector<int> Classify(std::vector<Instance> &test) override;

protected:

    void ComputeDistances(const std::vector<Instance> &data, const Instance &inst);

    std::vector<Instance> NearestInstances(const std::vector<Instance> &data, const Instance &inst);

    int k, labels;
    Metric *m;
    std::vector<Instance> train;
    std::vector<Distances> dist;
    std::vector<int> vClasses;
};