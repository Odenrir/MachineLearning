/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "ISRT.h"

ISRT::ISRT(float t, Metric &m) {
    this->t = t;
    this->m = &m;
}

void ISRT::Init() {

}

void ISRT::Clear() {
    this->dist.clear();
}

std::vector<Instance> ISRT::DoSelection(const std::vector<Instance> &dataset) {
    std::vector<std::vector<Instance>> categories = Utils::SplitDataSetIntoClasses(dataset);
    std::vector<Instance> relevants = std::vector<Instance>(categories.size());
    std::vector<std::vector<Instance>> borders = std::vector<std::vector<Instance>>(categories.size());
    std::vector<Relevance> relevances;
    std::list<Instance> temp;
    std::vector<Instance> selected;

    if (!dataset.empty()) {
        this->start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < categories.size(); i++) {
            relevances = this->ComputeRelevances(categories[i]);
            relevants[i] = this->SelectRelevant(relevances, categories[i]);
            std::vector<Relevance>().swap(relevances);
        }

        for (int i = 0; i < categories.size(); i++) {
            borders[i] = this->FindBorders(relevants[i], categories[i]);
        }
        this->stop = std::chrono::high_resolution_clock::now();

        for (const auto &relevant: relevants) {
            temp.push_back(relevant);
        }

        for (const auto &border: borders) {
            for (const auto &instance: border) {
                auto it = std::find(temp.begin(), temp.end(), instance);
                if (it == temp.end()) {
                    temp.push_back(instance);
                }
            }
        }

        selected = std::vector<Instance>(temp.begin(), temp.end());
    } else {
        this->start = std::chrono::high_resolution_clock::now();
        this->stop = std::chrono::high_resolution_clock::now();
    }
    return selected;
}

std::vector<Relevance> ISRT::ComputeRelevances(const std::vector<Instance> &data) {
    std::vector<Relevance> relevances(data.size());
    double distance;
    int x, y;
    std::string key;
    for (int i = 0; i < data.size(); i++) {
        relevances[i] = Relevance(i, 0);
    }
    for (int i = 0; i < data.size() - 1; i++) {
        x = data[i].GetID();
        for (int j = i + 1; j < data.size(); j++) {
            y = data[j].GetID();
            if (x < y) {
                key = std::to_string(x) + "," + std::to_string(y);
            } else {
                key = std::to_string(y) + "," + std::to_string(x);
            }

            if (this->dist.count(key) == 0) {
                this->dist[key] = Distances(x, y, this->m->Distance(data[i], data[j]));
            }
            distance = this->dist[key].val;
            relevances[i].val += distance;
            relevances[j].val += distance;
        }
    }
    return relevances;
}

Instance ISRT::SelectRelevant(std::vector<Relevance> &relevances, const std::vector<Instance> &data) {
    std::sort(relevances.begin(), relevances.end());
    Instance selected;
    selected = data[relevances[0].id];
    return selected;
}

std::vector<Instance> ISRT::FindBorders(const Instance &relevant, const std::vector<Instance> &category) {
    std::vector<Instance> borders;
    std::list<Instance> temp;
    int x = relevant.GetID(), y;
    std::string key;
    double mean = 0.0, meanSquares = 0.0, stdDev, variance, threshold;
    for (int i = 0; i < category.size(); i++) {
        y = category[i].GetID();
        if (x != y) {
            if (x < y) {
                key = std::to_string(x) + "," + std::to_string(y);
            } else {
                key = std::to_string(y) + "," + std::to_string(x);
            }
            mean += this->dist[key].val;
            meanSquares += std::pow(this->dist[key].val, 2);
        }
    }
    mean = mean / (category.size() - 1);
    meanSquares = meanSquares / (category.size() - 1);
    stdDev = meanSquares - std::pow(mean, 2);
    variance = std::sqrt(stdDev);

    for (int i = 0; i < category.size(); i++) {
        y = category[i].GetID();
        if (x != y) {
            if (x < y) {
                key = std::to_string(x) + "," + std::to_string(y);
            } else {
                key = std::to_string(y) + "," + std::to_string(x);
            }
            threshold = variance * this->t;
            if (this->dist[key].val > (mean - threshold) &&
                this->dist[key].val < (mean + threshold)) {
                temp.push_back(category[i]);
            }
        }
    }

    borders = std::vector<Instance>(temp.begin(), temp.end());
    return borders;
}


