/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_KMEANS_H
#define MACHINELEARNING_KMEANS_H

#include "Clustering.h"
#include "Metrics/Metric.h"
#include "Misc/Types.h"
#include "Misc/Utils.h"
#include <random>

class kMeans : public Clustering {
public:
    kMeans(int k, Metric &m);

    kMeans(int k, int it, Metric &m);

    kMeans(int k, int it, int seed, Metric &m);

    void Init() override;

    std::vector<std::vector<Instance>> BuildClustering(const std::vector<Instance> &dataset) override;

    std::vector<Instance> GetCentroids();

protected:

    Instance ComputeCentroid(const std::list<Instance> &cluster);

    int k, it, seed, featuresSize;
    std::mt19937 gen;
    std::uniform_int_distribution<> distr;
    Metric *m;
    std::vector<Instance> centroids;
};


#endif //MACHINELEARNING_KMEANS_H
