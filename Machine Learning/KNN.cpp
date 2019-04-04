#include "KNN.h"

KNN::KNN(int k, Metric &m) {
    this->k = k;
    this->m = &m;
    this->labels = 0;
}

void KNN::Init() {
}

void KNN::Learn(const std::vector<Instance> &train) {
    std::vector<int> descriptor(train[0].GetDescriptor());
    this->labels = descriptor[descriptor.size() - 1];
    this->vClasses = std::vector<int>(this->labels, 0);
    this->train = train;
}

int KNN::Classify(Instance &inst) {
    std::vector<int>().swap(this->vClasses);
    this->vClasses = std::vector<int>(this->labels, 0);
    std::vector<Instance> nearest;

    nearest = this->NearestInstances(this->train, inst);

    for (auto instance: nearest) {
        this->vClasses[instance.GetClass()]++;
    }

    std::vector<Instance>().swap(nearest);
    return Utils::Max(this->vClasses);
}

std::vector<Instance> KNN::NearestInstances(const std::vector<Instance> &data, const Instance &inst) {
    std::vector<Instance> result;
    this->ComputeDistances(data, inst);
    std::sort(this->dist.begin(), this->dist.end());
    if (this->k > data.size()) {
        result = std::vector<Instance>(data.size());
    } else {
        result = std::vector<Instance>(this->k);
    }
    for (int i = 0; i < this->k && i < data.size(); i++) {
        result[i] = data[this->dist[i].x];
    }

    return result;
}

const std::vector<int> KNN::Classify(std::vector<Instance> &test) {
    std::vector<int> predict(test.size());
    for (int i = 0; i < test.size(); i++) {
        predict[i] = this->Classify(test[i]);
    }
    return predict;
}

void KNN::ComputeDistances(const std::vector<Instance> &data, const Instance &inst) {
    std::vector<Distances>().swap(dist);
    this->dist = std::vector<Distances>(data.size());
    for (int i = 0; i < data.size(); i++) {
        this->dist[i] = Distances(i, -1, this->m->Distance(data[i], inst));
    }
}
