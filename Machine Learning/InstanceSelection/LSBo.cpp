#include "LSBo.h"

LSBo::LSBo(Metric &m) {
    this->m = &m;
}

void LSBo::Init() {

}

void LSBo::Clear() {
    this->dist.clear();
}

std::vector<Instance> LSBo::DoSelection(const std::vector<Instance> &dataset) {
    std::list<int> temp;
    std::vector<Instance> selected;
    std::map<int, LocalSet> LS;
    std::string key;
    int id, enemyid, x, y, counter = 0;
    double distance;
    std::vector<std::vector<Instance>> categories;

    if (!dataset.empty()) {
        LSSm filter(*this->m);
        this->start = std::chrono::high_resolution_clock::now();
        std::vector<Instance> filtered = filter.DoSelection(dataset);
        categories = Utils::SplitDataSetIntoClasses(filtered);
        this->dist = filter.GetDistances();
        filter.Clear();

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
        std::sort(auxLS.begin(), auxLS.end());
        bool contained;
        for (const LocalSet &l: auxLS) {
            contained = false;
            for (auto pair: l.LS) {
                auto it = std::find(temp.begin(), temp.end(), pair.first);
                if (it != temp.end()) {
                    contained = true;
                }
            }
            if (!contained) {
                temp.push_back(l.id);
            }
        }

        this->stop = std::chrono::high_resolution_clock::now();

        selected = std::vector<Instance>(temp.size());
        for (int x: temp) {
            selected[counter] = Utils::FindInstance(x, filtered);
            counter++;
        }
    } else {
        this->start = std::chrono::high_resolution_clock::now();
        this->stop = std::chrono::high_resolution_clock::now();
    }
    for (auto localset: LS) {
        localset.second.LS.clear();
    }
    LS.clear();
    std::cout << std::to_string(this->GetRuntime()) << "\n";
    return selected;
}

std::map<std::string, Distances> LSBo::GetDistances() {
    return this->dist;
}

int LSBo::FindNearestEnemy(const Instance &reference, const std::vector<std::vector<Instance>> &categories) {
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
