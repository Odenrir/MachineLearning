#include "LSSm.h"

LSSm::LSSm(Metric &m) {
    this->m = &m;
}

void LSSm::Init() {

}

void LSSm::Clear() {
    this->dist.clear();
}

std::vector<Instance> LSSm::DoSelection(const std::vector<Instance> &dataset) {
    std::list<Instance> temp;
    std::vector<Instance> selected;
    std::map<int, int> u;
    std::map<int, int> h;
    std::map<int, LocalSet> LS;
    std::string key;
    int id, enemyid, x, y;
    double distance;
    std::vector<std::vector<Instance>> categories = Utils::SplitDataSetIntoClasses(dataset);

    if (!dataset.empty()) {
        for (const auto &instance: dataset) {
            u[instance.GetID()] = 0;
            h[instance.GetID()] = 0;
        }

        this->start = std::chrono::high_resolution_clock::now();
        for (const auto &category: categories) {
            for (const auto &instance: category) {
                id = instance.GetID();
                enemyid = this->FindNearestEnemy(instance, categories);
                h[enemyid]++;
                if (id < enemyid) {
                    key = std::to_string(id) + "," + std::to_string(enemyid);
                } else {
                    key = std::to_string(enemyid) + "," + std::to_string(id);
                }
                distance = this->dist[key].val;
                LS[id] = LocalSet(id, enemyid, distance, instance.GetClass());
            }
        }

        for (auto category: categories) {
            for (int i = 0; i < category.size() - 1; i++) {
                x = category[i].GetID();
                u[x]++;
                for (int j = i + 1; j < category.size(); j++) {
                    y = category[j].GetID();
                    if (x < y) {
                        key = std::to_string(x) + "," + std::to_string(y);
                    } else {
                        key = std::to_string(y) + "," + std::to_string(x);
                    }
                    if (this->dist.count(key) == 0) {
                        this->dist[key] = Distances(x, y, this->m->Distance(category[i], category[j]));
                    }
                    distance = this->dist[key].val;
                    if (LS[x].tryAppend(y, distance)) {
                        u[y]++;
                    }
                    if (LS[y].tryAppend(x, distance)) {
                        u[x]++;
                    }
                }
            }
        }

        for (const auto &instance: dataset) {
            if (h[instance.GetID()] < u[instance.GetID()]) {
                temp.push_back(instance);
            }
        }
        this->stop = std::chrono::high_resolution_clock::now();

        selected = std::vector<Instance>(temp.begin(), temp.end());
    } else {
        this->start = std::chrono::high_resolution_clock::now();
        this->stop = std::chrono::high_resolution_clock::now();
    }
    for (auto localset: LS) {
        localset.second.LS.clear();
    }
    LS.clear();
    return selected;
}

std::map<std::string, Distances> LSSm::GetDistances() {
    return this->dist;
}

int LSSm::FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories) {
    int y, id = -1;
    int x = reference.GetID();
    double minDist = std::numeric_limits<double>::max();
    double distance;
    std::string key;

    for (auto category: categories) {
        if (category[0].GetClass() != reference.GetClass()) {
            for (const auto &instance: category) {
                y = instance.GetID();
                if (x < y) {
                    key = std::to_string(x) + "," + std::to_string(y);
                } else {
                    key = std::to_string(y) + "," + std::to_string(x);
                }

                if (this->dist.count(key) == 0) {
                    this->dist[key] = Distances(x, y, this->m->Distance(reference, instance));
                }
                distance = this->dist[key].val;
                if (distance < minDist) {
                    minDist = distance;
                    id = y;
                }
            }
        }
    }

    return id;
}
