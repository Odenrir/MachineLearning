#include "IBR.h"

IBR::IBR(int k, float hiR, float meR, float loR, Metric &m) {
    this->k = k;
    this->hiR = hiR;
    this->meR = meR;
    this->loR = loR;
    this->m = &m;
}

void IBR::Init() {

}

void IBR::Clear() {
    this->dist.clear();
}

std::vector<Instance> IBR::DoSelection(const std::vector<Instance> &dataset) {
    std::vector<std::vector<Instance>> categories;
    std::vector<std::vector<int>> rankedIds;
    std::vector<Relevance> ranks;
    std::list<Instance> temp;
    std::vector<Instance> selected;
    Instance aux;

    if (!dataset.empty()) {
        ENN filter(*m, this->k);
        this->start = std::chrono::high_resolution_clock::now();
        std::vector<Instance> filtered = filter.DoSelection(dataset);
        categories = Utils::SplitDataSetIntoClasses(filtered);
        rankedIds = std::vector<std::vector<int>>(categories.size());
        this->dist = filter.GetDistances();

        for (int i = 0; i < categories.size(); i++) {
            ranks = this->ComputeRanks(categories[i], categories);
            rankedIds[i] = this->SelectInstances(ranks, categories[i]);
            std::vector<Relevance>().swap(ranks);
        }

        this->stop = std::chrono::high_resolution_clock::now();

        for (const auto &rank: rankedIds) {
            for (int id: rank) {
                aux = Utils::FindInstance(id, filtered);
                auto it = std::find(temp.begin(), temp.end(), aux);
                if (it == temp.end()) {
                    temp.push_back(aux);
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

std::vector<Relevance>
IBR::ComputeRanks(const std::vector<Instance> &category, const std::vector<std::vector<Instance>> &categories) {
    std::vector<Relevance> ranks(category.size());
    for (int i = 0; i < category.size(); i++) {
        ranks[i] = Relevance(category[i].GetID(), this->ComputeRank(category[i], categories));
    }
    return ranks;
}

double IBR::ComputeRank(const Instance &reference, const std::vector<std::vector<Instance>> &categories) {
    double result = 0;
    std::list<Distances> auxDist;
    std::vector<Distances> vecDist;
    for (auto category: categories) {
        if (!category.empty()) {
            if (category[0].GetClass() != reference.GetClass()) {
                for (const auto &instance: category) {
                    auxDist.push_back(this->Similarity(reference, instance));
                }
            }
        }
    }
    vecDist = std::vector<Distances>(auxDist.begin(), auxDist.end());
    std::sort(vecDist.rbegin(), vecDist.rend());

    for (int i = 0; i < this->k; i++) {
        result += vecDist[i].val;
    }
    std::list<Distances>().swap(auxDist);
    std::vector<Distances>().swap(vecDist);
    result /= this->k;
    return result;
}

std::vector<int> IBR::SelectInstances(std::vector<Relevance> &ranks, const std::vector<Instance> &data) {
    std::sort(ranks.rbegin(), ranks.rend());
    int r1 = (this->hiR / 100) * data.size();
    int r2 = (this->meR / 100) * data.size();
    int r3 = (this->loR / 100) * data.size();
    int med = (data.size() / 2) - (r2 / 2);
    int low = data.size() - r3;
    std::vector<int> selected(r1 + r2 + r3);
    for (int i = 0; i < r1; i++) {
        selected[i] = ranks[i].id;
    }

    for (int i = 0; i < r2; i++) {
        selected[r1 + i] = ranks[med + i].id;
    }

    for (int i = 0; i < r3; i++) {
        selected[r1 + r2 + i] = ranks[low + i].id;
    }

    return selected;
}

Distances IBR::Similarity(const Instance &x, const Instance &y) {
    int id1, id2;
    std::string key;
    id1 = x.GetID();
    id2 = y.GetID();
    if (id1 < id2) {
        key = std::to_string(id1) + "," + std::to_string(id2);
    } else {
        key = std::to_string(id2) + "," + std::to_string(id1);
    }

    if (this->dist.count(key) == 0) {
        this->dist[key] = Distances(id1, id2, this->m->Distance(x, y));
    }
    return {id1, id2, 1 - this->dist[key].val};
}
