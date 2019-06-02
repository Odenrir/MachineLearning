#include "PSR.h"

PSR::PSR(float ret, Metric &m) {
    this->ret = ret;
    this->m = &m;
}

void PSR::Init() {

}

void PSR::Clear() {
    this->dist.clear();
}

std::vector<Instance> PSR::DoSelection(const std::vector<Instance> &dataset) {
    std::vector<std::vector<Instance>> classes = Utils::SplitDataSetIntoClasses(dataset);
    std::vector<std::vector<Instance>> relevants = std::vector<std::vector<Instance>>(classes.size());
    std::vector<std::vector<Instance>> borders = std::vector<std::vector<Instance>>(classes.size());
    std::vector<Relevance> relevances;
    std::list<Instance> temp;
    std::vector<Instance> selected;

    if (!dataset.empty()) {
        this->start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < classes.size(); i++) {
            relevances = this->ComputeRelevances(classes[i]);
            relevants[i] = this->SelectRelevants(relevances, classes[i]);
            std::vector<Relevance>().swap(relevances);
        }

        for (int i = 0; i < classes.size(); i++) {
            borders[i] = this->FindBorders(relevants[i], classes);
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

std::vector<Relevance> PSR::ComputeRelevances(const std::vector<Instance> &data) {
    std::vector<Relevance> relevances(data.size());
    double auxDist;
    for (int i = 0; i < data.size(); i++) {
        relevances[i] = Relevance(i, 0);
    }
    for (int i = 0; i < data.size() - 1; i++) {
        for (int j = i + 1; j < data.size(); j++) {
            auxDist = this->m->Distance(data[i], data[j]);
            relevances[i].val += auxDist;
            relevances[j].val += auxDist;
        }
    }
    return relevances;
}

std::vector<Instance>
PSR::SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data) {
    std::sort(relevances.begin(), relevances.end());
    int elements = (this->ret / 100) * data.size();
    std::vector<Instance> selected(elements);
    for (int i = 0; i < elements; i++) {
        selected[i] = data[relevances[i].id];
    }
    return selected;
}

std::vector<Instance>
PSR::FindBorders(const std::vector<Instance> &relevants, const std::vector<std::vector<Instance>> &complement) {
    std::vector<Instance> borders;
    std::list<Instance> temp;
    Instance nearest;
    double minDist, distance;
    int x, y;
    std::string key;
    for (const auto &relevant: relevants) {
        x = relevant.GetID();
        for (const auto &category : complement) {
            if (!category.empty() && category[0].GetClass() != relevant.GetClass()) {
                minDist = std::numeric_limits<double>::max();
                for (const auto &instance : category) {
                    y = instance.GetID();
                    if (x < y) {
                        key = std::to_string(x) + "," + std::to_string(y);
                    } else {
                        key = std::to_string(y) + "," + std::to_string(x);
                    }

                    if (this->dist.count(key) == 0) {
                        this->dist[key] = Distances(x, y, this->m->Distance(relevant, instance));
                    }
                    distance = this->dist[key].val;
                    if (distance < minDist) {
                        minDist = distance;
                        nearest = instance;
                    }
                }
                temp.push_back(nearest);
            }
        }
    }
    borders = std::vector<Instance>(temp.begin(), temp.end());
    return borders;
}
