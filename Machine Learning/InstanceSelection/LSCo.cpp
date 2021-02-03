/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "LSCo.h"

LSCo::LSCo(Metric &m) {
    this->m = &m;
}

void LSCo::Init() {

}

void LSCo::Clear() {
    this->dist.clear();
}

std::vector<Instance> LSCo::DoSelection(const std::vector<Instance> &dataset) {
    std::list<Instance> temp;
    std::vector<Instance> selected;
    std::map<int, LocalSet> LS;
    std::string key;
    int id, enemyid, x, y;
    double distance;
    std::vector<std::vector<Instance>> categories;

    if (!dataset.empty()) {
        LSSm filter(*this->m);
        this->start = std::chrono::high_resolution_clock::now();
        std::vector<Instance> filtered = filter.DoSelection(dataset);
        this->dist = filter.GetDistances();
        filter.Clear();
        categories = Utils::SplitDataSetIntoClasses(filtered);

        for (const auto &category: categories) {
            for (const auto &instance: category) {
                id = instance.GetID();
                enemyid = this->FindNearestEnemy(instance, categories);
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
                    LS[x].tryAppend(y, distance);
                    LS[y].tryAppend(x, distance);
                }
            }
        }

        std::vector<LocalSet> auxLS;
        MapToVec(LS, auxLS);
        std::vector<int> medoids = this->LSClustering(auxLS);
        std::list<int> medoidsNI;
        for (int medoid: medoids) {
            if (LS[medoid].cardinality > 1) {
                medoidsNI.push_back(medoid);
            }
        }

        this->stop = std::chrono::high_resolution_clock::now();

        for (int medoid: medoidsNI) {
            temp.push_back(Utils::FindInstance(medoid, filtered));
        }

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

std::map<std::string, Distances> LSCo::GetDistances() {
    return this->dist;
}

std::vector<int> LSCo::LSClustering(std::vector<LocalSet> &ls) {
    std::vector<int> result;
    std::list<int> medoidsIDs;
    std::map<int, int> medoidIndex;
    std::sort(ls.rbegin(), ls.rend());
    //std::sort(ls.begin(), ls.end());
    bool covered;
    for (int i = 0; i < ls.size(); i++) {
        covered = false;
        for (int id: medoidsIDs) {
            if (ls[medoidIndex[id]].isIn(ls[i].id)) {
                covered = true;
            }
        }
        if (!covered) {
            medoidsIDs.push_back(ls[i].id);
            medoidIndex[ls[i].id] = i;
        }
    }
    result = std::vector<int>(medoidsIDs.begin(), medoidsIDs.end());
    return result;
}

int LSCo::FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories) {
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
