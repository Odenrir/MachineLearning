/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "ENN.h"

ENN::ENN(Metric &m, int k) {
    this->m = &m;
    this->k = k;
    this->labels = 0;
}

void ENN::Init() {

}

void ENN::Clear() {
    this->dist.clear();
    this->idIndex.clear();
}

std::vector<Instance> ENN::DoSelection(const std::vector<Instance> &dataset) {
    std::list<Instance> temp;
    std::vector<Instance> selected;

    if (!dataset.empty()) {
        for (int i = 0; i < dataset.size(); i++) {
            this->idIndex[dataset[i].GetID()] = i;
        }
        std::vector<int> descriptor(dataset[0].GetDescriptor());
        this->labels = descriptor[descriptor.size() - 1];
        this->vClasses = std::vector<int>(this->labels, 0);
        this->start = std::chrono::high_resolution_clock::now();

        for (const auto &instance: dataset) {
            int cat = this->Classify(instance, dataset);
            //if (instance.GetClass() == this->Classify(instance, dataset)) {
            if (instance.GetClass() == cat) {
                temp.push_back(instance);
            }
        }

        this->stop = std::chrono::high_resolution_clock::now();

        selected = std::vector<Instance>(temp.begin(), temp.end());
    } else {
        this->start = std::chrono::high_resolution_clock::now();
        this->stop = std::chrono::high_resolution_clock::now();
    }
    return selected;
}

std::map<std::string, Distances> ENN::GetDistances() {
    return this->dist;
}

std::vector<Distances> ENN::ComputeDistances(const std::vector<Instance> &data, const Instance &inst) {
    std::vector<Distances> aux(data.size() - 1);
    int x, y, counter = 0;
    std::string key;
    x = inst.GetID();
    for (const auto &instance: data) {
        y = instance.GetID();
        if (x != y) {
            if (x < y) {
                key = std::to_string(x) + "," + std::to_string(y);
            } else {
                key = std::to_string(y) + "," + std::to_string(x);
            }
            if (this->dist.count(key) == 0) {
                this->dist[key] = Distances(x, y, this->m->Distance(instance, inst));
            }
            aux[counter] = this->dist[key];
            counter++;
        }
    }
    return aux;
}

int ENN::Classify(const Instance &inst, const std::vector<Instance> &train) {
    std::vector<int>().swap(this->vClasses);
    this->vClasses = std::vector<int>(this->labels, 0);
    std::vector<Instance> nearest;
    nearest = this->NearestInstances(train, inst);
    for (const auto &instance: nearest) {
        this->vClasses[instance.GetClass()]++;
    }
    std::vector<Instance>().swap(nearest);
    return Utils::Max(this->vClasses);
}

std::vector<Instance> ENN::NearestInstances(const std::vector<Instance> &data, const Instance &inst) {
    std::vector<Instance> result;
    std::vector<Distances> nearestDist = this->ComputeDistances(data, inst);
    std::sort(nearestDist.begin(), nearestDist.end());
    if (this->k > data.size()) {
        result = std::vector<Instance>(data.size());
    } else {
        result = std::vector<Instance>(this->k);
    }
    for (int i = 0; i < this->k && i < data.size(); i++) {
        result[i] = data[this->idIndex[nearestDist[i].y]];
    }
    std::vector<Distances>().swap(nearestDist);
    return result;
}
