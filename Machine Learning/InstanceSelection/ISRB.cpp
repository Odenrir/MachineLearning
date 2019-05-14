#include <cmath>

#include "ISRB.h"

ISRB::ISRB(float ret, Metric &m) {
    this->ret = ret;
    this->m = &m;
    this->featuresSize = 0;
}

void ISRB::Init() {

}

void ISRB::Clear() {

}


std::vector<Instance> ISRB::DoSelection(const std::vector<Instance> &dataset) {
    std::vector<std::vector<Instance>> categories = Utils::SplitDataSetIntoClasses(dataset);
    std::vector<std::vector<Instance>> relevants = std::vector<std::vector<Instance>>(categories.size());
    std::vector<Instance> mostRelevantsClass = std::vector<Instance>(categories.size());
    std::vector<std::vector<Instance>> borders = std::vector<std::vector<Instance>>(categories.size());
    std::vector<Relevance> relevances;
    std::list<Instance> temp;
    std::vector<Instance> selected;

    if (!dataset.empty()) {
        this->start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < categories.size(); i++) {
            relevances = this->ComputeRelevances(categories[i]);
            relevants[i] = this->SelectRelevants(relevances, categories[i]);
            mostRelevantsClass[i] = relevants[i][0];
            std::vector<Relevance>().swap(relevances);
        }

        for (int i = 0; i < categories.size(); i++) {
            borders[i] = this->FindBorders(mostRelevantsClass, categories);
        }
        this->stop = std::chrono::high_resolution_clock::now();

        for (const auto &rel: relevants) {
            for (const auto &instance: rel) {
                temp.push_back(instance);
            }
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

std::vector<Relevance> ISRB::ComputeRelevances(const std::vector<Instance> &data) {
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

std::vector<Instance>
ISRB::SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data) {
    std::sort(relevances.begin(), relevances.end());
    int elements = std::ceil((this->ret / 100) * data.size());
    std::vector<Instance> selected(elements);
    for (int i = 0; i < elements; i++) {
        selected[i] = data[relevances[i].id];
    }
    return selected;
}

std::vector<Instance>
ISRB::FindBorders(const std::vector<Instance> &topRelevants, const std::vector<std::vector<Instance>> &categories) {
    std::vector<Instance> borders;
    std::vector<Instance> references(2);
    std::list<Instance> temp;
    Instance intersection, categoryRelevant;
    int x, y;
    std::string key;
    for (const auto &relevant: topRelevants) {
        references[0] = relevant;
        for (const auto &category: categories) {
            if (relevant.GetClass() != category[0].GetClass()) {
                categoryRelevant = this->FindRelevant(topRelevants, category[0].GetClass());
                references[1] = categoryRelevant;
                intersection = this->ComputeCentroid(references);
                x = categoryRelevant.GetID();
                for (const auto &instance: category) {
                    y = instance.GetID();
                    if (x < y) {
                        key = std::to_string(x) + "," + std::to_string(y);
                    } else {
                        key = std::to_string(y) + "," + std::to_string(x);
                    }
                    if (this->dist[key].val > this->m->Distance(instance, intersection)) {
                        temp.push_back(instance);
                    }
                }
            }
        }
    }

    borders = std::vector<Instance>(temp.begin(), temp.end());
    return borders;
}

Instance ISRB::ComputeCentroid(const std::vector<Instance> &data) {
    std::vector<float> featC;
    if (!data.empty()) {
        featC = std::vector<float>(data[0].CountNumericFeatures(), 0);
        for (const auto &instance: data) {
            for (int i = 0; i < featC.size(); i++) {
                featC[i] += instance.GetNumericFeature(i);
            }
        }

        for (float &feat : featC) {
            feat /= data.size();
        }
    } else {
        featC = std::vector<float>(this->featuresSize, std::numeric_limits<float>::max());
    }
    return Instance(featC);
}

Instance ISRB::FindRelevant(const std::vector<Instance> &relevants, int category) {
    for (auto relevant: relevants) {
        if (relevant.GetClass() == category) {
            return relevant;
        }
    }
    return Instance();
}
