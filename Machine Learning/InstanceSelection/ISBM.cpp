#include "ISBM.h"


ISBM::ISBM(float ret, Metric &m) {
    this->ret = ret;
    this->m = &m;
    this->featuresSize = 0;
}

void ISBM::Init() {

}

void ISBM::Clear() {

}


std::vector<Instance> ISBM::DoSelection(const std::vector<Instance> &dataset) {
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

std::vector<Instance> ISBM::GetIntersectionRepresentatives() {
    return this->bRrepresentatives;
}

std::vector<Relevance> ISBM::ComputeRelevances(const std::vector<Instance> &data) {
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
ISBM::SelectRelevants(std::vector<Relevance> &relevances, const std::vector<Instance> &data) {
    std::sort(relevances.begin(), relevances.end());
    int elements = std::ceil((this->ret / 100) * data.size());
    std::vector<Instance> selected(elements);
    for (int i = 0; i < elements; i++) {
        selected[i] = data[relevances[i].id];
    }
    return selected;
}

std::vector<Instance>
ISBM::FindBorders(const std::vector<Instance> &topRelevants, const std::vector<std::vector<Instance>> &categories) {
    std::vector<Instance> borders;
    std::list<Instance> temp, bRep;
    Instance intersection, categoryRelevant;
    int x, y, z, indexCat, indexRel;
    std::string key1, key2;
    for (const auto &category: categories) {
        for (const auto &relevant: topRelevants) {
            if (category[0].GetClass() != relevant.GetClass()) {
                intersection = this->ComputeBorderRelevant(category, relevant);
                indexCat = Utils::SameClass(category[0].GetClass(), categories);
                indexRel = Utils::SameClass(category[0].GetClass(), topRelevants);
                z = intersection.GetID();
                x = topRelevants[indexRel].GetID();
                bRep.push_back(intersection);
                for (const auto &instance: categories[indexCat]) {
                    y = instance.GetID();
                    if (x < y) {
                        key1 = std::to_string(x) + "," + std::to_string(y);
                    } else {
                        key1 = std::to_string(y) + "," + std::to_string(x);
                    }
                    if (z < y) {
                        key2 = std::to_string(z) + "," + std::to_string(y);
                    } else {
                        key2 = std::to_string(y) + "," + std::to_string(z);
                    }
                    if (this->dist[key2].val < this->dist[key1].val) {
                        temp.push_back(instance);
                    }
                }
            }
        }
    }

    this->bRrepresentatives = std::vector<Instance>(bRep.begin(), bRep.end());
    borders = std::vector<Instance>(temp.begin(), temp.end());
    return borders;
}

Instance ISBM::ComputeBorderRelevant(const std::vector<Instance> &data, const Instance &representative) {
    std::vector<Relevance> borderRel(data.size());
    int x, y, id;
    double distance;
    std::string key;
    x = representative.GetID();
    for (int i = 0; i < data.size(); i++) {
        y = data[i].GetID();
        if (x < y) {
            key = std::to_string(x) + "," + std::to_string(y);
        } else {
            key = std::to_string(y) + "," + std::to_string(x);
        }

        if (this->dist.count(key) == 0) {
            this->dist[key] = Distances(x, y, this->m->Distance(data[i], representative));
        }
        distance = this->dist[key].val;
        borderRel[i] = Relevance(i, distance);
    }
    std::sort(borderRel.begin(), borderRel.end());
    id = borderRel[0].id;
    std::vector<Relevance>().swap(borderRel);
    return data[id];
}

Instance ISBM::FindRelevant(const std::vector<Instance> &relevants, int category) {
    for (auto relevant: relevants) {
        if (relevant.GetClass() == category) {
            return relevant;
        }
    }
    return Instance();
}
