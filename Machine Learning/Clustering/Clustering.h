#ifndef MACHINELEARNING_CLUSTERING_H
#define MACHINELEARNING_CLUSTERING_H
#include "Misc/Instance.h"

class Clustering {
public:
    virtual void Init() = 0;

    virtual std::vector<std::vector<Instance>> BuildClustering(const std::vector<Instance> &dataset) = 0;


};
#endif //MACHINELEARNING_CLUSTERING_H
