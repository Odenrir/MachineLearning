#include "kMeans.h"

kMeans::kMeans(int k, Metric &m) {
    this->k = k;
    this->it = std::numeric_limits<int>::infinity();
    this->seed = 0;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(0);
    this->centroids = std::vector<Instance>(k);
}

kMeans::kMeans(int k, int it, Metric &m) {
    this->k = k;
    this->it = it;
    this->seed = 0;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(0);
    this->centroids = std::vector<Instance>(k);
}

kMeans::kMeans(int k, int it, int seed, Metric &m) {
    this->k = k;
    this->it = it;
    this->seed = seed;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(seed);
    this->centroids = std::vector<Instance>(k);
}

void kMeans::Init() {

}


std::vector<std::vector<Instance>> kMeans::BuildClustering(const std::vector<Instance> &dataset) {
    this->start = std::chrono::high_resolution_clock::now();
    if (dataset.empty()) {
        this->stop = std::chrono::high_resolution_clock::now();
        return std::vector<std::vector<Instance>>();
    }
    this->distr = std::uniform_int_distribution<>(0, dataset.size() - 1);
    this->featuresSize = dataset[0].CountNumericFeatures();
    std::vector<Instance> oldMeans(this->k);
    std::vector<int> indices(this->k, -1);
    std::vector<std::list<Instance>> clusters(this->k);
    double minDist, auxDist;
    int minPos = -1;
    int randomNum = this->distr(this->gen);

    for (int i = 0; i < this->k; i++) {
        while (std::find(indices.begin(), indices.end(), randomNum) != indices.end()) {
            randomNum = this->distr(this->gen);
        }
        indices[i] = randomNum;
        this->centroids[i] = dataset[indices[i]];
    }
    do {
        for (int i = 0; i < this->k; i++) {
            clusters[i].clear();
        }
        for (const auto &instance : dataset) {
            minDist = std::numeric_limits<double>::max();
            for (int i = 0; i < this->k; i++) {
                auxDist = m->Distance(instance, this->centroids[i]);
                if (auxDist < minDist) {
                    minDist = auxDist;
                    minPos = i;
                }
            }
            clusters[minPos].push_back(instance);
        }
        oldMeans.swap(this->centroids);
        std::vector<Instance>().swap(this->centroids);
        this->centroids = std::vector<Instance>(this->k);

        for (int i = 0; i < this->k; i++) {
            this->centroids[i] = this->ComputeCentroid(clusters[i]);
        }

    } while (!Utils::CompareInstances(this->centroids, oldMeans));
    this->stop = std::chrono::high_resolution_clock::now();
    std::vector<std::vector<Instance>> result(this->k);
    for (int i = 0; i < this->k; i++) {
        result[i] = std::vector<Instance>(clusters[i].begin(), clusters[i].end());
    }
    return result;
}

std::vector<Instance> kMeans::GetCentroids() {
    return this->centroids;
}

Instance kMeans::ComputeCentroid(const std::list<Instance> &cluster) {
    std::vector<float> featC;
    if (!cluster.empty()) {
        featC = std::vector<float>(cluster.back().CountNumericFeatures(), 0);
        for (const auto &instance: cluster) {
            for (int i = 0; i < featC.size(); i++) {
                featC[i] += instance.GetNumericFeature(i);
            }
        }

        for (float &feat : featC) {
            feat /= cluster.size();
        }
    } else {
        featC = std::vector<float>(this->featuresSize, std::numeric_limits<float>::max());
    }
    return Instance(featC);
}


