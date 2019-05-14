#include "kNN.h"

kNN::kNN(int k, Metric &m) {
    this->k = k;
    this->m = &m;
    this->labels = 0;
}

void kNN::Init() {
}

void kNN::Learn(const std::vector<Instance> &train) {
    std::vector<int> descriptor(train[0].GetDescriptor());
    this->labels = descriptor[descriptor.size() - 1];
    this->vClasses = std::vector<int>(this->labels, 0);
    this->startLearning = std::chrono::high_resolution_clock::now();
    this->train = train;
    this->stopLearning = std::chrono::high_resolution_clock::now();
}

int kNN::Classify(const Instance &inst) {
    this->startClassifyingSingle = std::chrono::high_resolution_clock::now();
    std::vector<int>().swap(this->vClasses);
    this->vClasses = std::vector<int>(this->labels, 0);
    std::vector<Instance> nearest;
    nearest = this->NearestInstances(this->train, inst);
    for (const auto &instance: nearest) {
        this->vClasses[instance.GetClass()]++;
    }
    std::vector<Instance>().swap(nearest);
    this->stopClassifyingSingle = std::chrono::high_resolution_clock::now();
    return Utils::Max(this->vClasses);
}

std::vector<Instance> kNN::NearestInstances(const std::vector<Instance> &data, const Instance &inst) {
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

const std::vector<int> kNN::Classify(const std::vector<Instance> &test) {
    std::vector<int> predict(test.size());
    this->startClassifyingMulti = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test.size(); i++) {
        predict[i] = this->Classify(test[i]);
    }
    this->stopClassifyingMulti = std::chrono::high_resolution_clock::now();
    return predict;
}

void kNN::ComputeDistances(const std::vector<Instance> &data, const Instance &inst) {
    std::vector<Distances>().swap(this->dist);
    this->dist = std::vector<Distances>(data.size());
    for (int i = 0; i < data.size(); i++) {
        this->dist[i] = Distances(i, -1, this->m->Distance(data[i], inst));
    }
}
