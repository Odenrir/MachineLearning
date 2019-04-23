#include "PAM.h"


PAM::PAM(int k, Metric &m) {
    this->k = k;
    this->it = std::numeric_limits<int>::infinity();
    this->seed = 0;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(0);
    this->medoids = std::vector<Instance>(k);
}

PAM::PAM(int k, int it, Metric &m) {
    this->k = k;
    this->it = it;
    this->seed = 0;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(0);
    this->medoids = std::vector<Instance>(k);
}

PAM::PAM(int k, int it, int seed, Metric &m) {
    this->k = k;
    this->it = it;
    this->seed = seed;
    this->featuresSize = 0;
    this->m = &m;
    this->gen = std::mt19937(seed);
    this->medoids = std::vector<Instance>(k);
}

void PAM::Init() {

}


std::vector<std::vector<Instance>> PAM::BuildClustering(const std::vector<Instance> &dataset) {
    if (dataset.empty()) {
        return std::vector<std::vector<Instance>>();
    }
    this->distr = std::uniform_int_distribution<>(0, dataset.size() - 1);
    this->featuresSize = dataset[0].CountNumericFeatures();
    std::vector<Instance> oldMeans(this->k);
    std::vector<Instance> auxCluster;
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
        this->medoids[i] = dataset[indices[i]];
    }
    do {
        for (int i = 0; i < this->k; i++) {
            clusters[i].clear();
        }
        for (const auto& instance : dataset) {
            minDist = std::numeric_limits<double>::max();
            for (int i = 0; i < this->k; i++) {
                auxDist = m->Distance(instance, this->medoids[i]);
                if (auxDist < minDist) {
                    minDist = auxDist;
                    minPos = i;
                }
            }
            clusters[minPos].push_back(instance);
        }
        oldMeans.swap(this->medoids);
        std::vector<Instance>().swap(this->medoids);
        this->medoids = std::vector<Instance>(this->k);

        for (int i = 0; i < this->k; i++) {
            std::vector<Instance>().swap(auxCluster);
            auxCluster = std::vector<Instance>(clusters[i].begin(), clusters[i].end());
            this->medoids[i] = this->ComputeMedoid(auxCluster);
        }

    } while (!Utils::CompareInstances(this->medoids, oldMeans));
    std::vector<std::vector<Instance>> result(this->k);
    for (int i = 0; i < this->k; i++) {
        result[i] = std::vector<Instance>(clusters[i].begin(), clusters[i].end());
    }
    return result;
}

std::vector<Instance> PAM::GetMedoids() {
    return this->medoids;
}

Instance PAM::ComputeMedoid(const std::vector<Instance> &cluster) {
    std::map<int, double> distSum;
    double distance, min = std::numeric_limits<double>::max();
    int x, y;
    std::string key;
    Instance representative;
    for (int i = 0; i < cluster.size() - 1; i++) {
        x = cluster[i].GetID();
        for (int j = i + 1; j < cluster.size(); j++) {
            y = cluster[j].GetID();
            if (x < y) {
                key = std::to_string(x) + "," + std::to_string(y);
            } else {
                key = std::to_string(y) + "," + std::to_string(x);
            }

            if (this->dist.count(key) == 0) {
                this->dist[key] = Distances(x, y, this->m->Distance(cluster[i], cluster[j]));
            }
            distance = this->dist[key].val;
            if (distSum.count(x) == 0) {
                distSum[x] = distance;
            } else {
                distSum[x] += distance;
            }
            if (distSum.count(y) == 0) {
                distSum[y] = distance;
            } else {
                distSum[y] += distance;
            }
        }
    }

    for (const auto& instance: cluster) {
        if (distSum[instance.GetID()] < min) {
            min = distSum[instance.GetID()];
            representative = instance;
        }
    }

    return representative;
}

