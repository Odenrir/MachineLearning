#ifndef MACHINELEARNING_PAM_H
#define MACHINELEARNING_PAM_H
#include "Clustering.h"
#include "Metrics/Metric.h"
#include "Misc/Types.h"
#include "Misc/Utils.h"
#include <random>

class PAM: public Clustering {
public:
    PAM(int k, Metric &m);

    PAM(int k, int it, Metric &m);

    PAM(int k, int it, int seed, Metric &m);

    void Init() override;

    std::vector<std::vector<Instance>> BuildClustering(const std::vector<Instance> &dataset) override;

    std::vector<Instance> GetMedoids();

protected:

    Instance ComputeMedoid(const std::vector<Instance>& cluster);

    int k, it, seed, featuresSize;
    std::mt19937 gen;
    std::uniform_int_distribution<> distr;
    Metric *m;
    std::vector<Instance> medoids;
    std::map<std::string, Distances> dist;
};


#endif //MACHINELEARNING_PAM_H
