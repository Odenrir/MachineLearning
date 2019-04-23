#include "PSC.h"

PSC::PSC(int t, Metric &m) {
    this->t = t;
    this->m = &m;
    this->c = 0;
}

void PSC::Init() {

}

std::vector<Instance> PSC::DoSelection(const std::vector<Instance> &dataset) {
    std::vector<Instance> selected;
    std::vector<Instance> clusterRepresentatives;
    std::vector<std::vector<Instance>> clusters;
    std::list<Instance> representatives;
    std::list<Instance> temp;
    std::list<std::vector<Instance>> borders;
    Instance nearest;
    double min, dist;
    bool numeric;

    if (!dataset.empty()) {
        std::vector<int> descriptor(dataset[0].GetDescriptor());
        this->c = t * descriptor[descriptor.size() - 1];
        numeric = dataset[0].IsNumeric();
        if (numeric) {
            kMeans kmeans(this->c, *this->m);
            clusters = kmeans.BuildClustering(dataset);
            clusterRepresentatives = kmeans.GetCentroids();
        } else {
            PAM pam(this->c, *this->m);
            clusters = pam.BuildClustering(dataset);
            clusterRepresentatives = pam.GetMedoids();
        }

        for (int i = 0; i < clusters.size(); i++) {
            if (!clusters[i].empty()) {
                if (this->IsHomogeneous(clusters[i])) {
                    if (numeric) {
                        nearest = Instance();
                        min = std::numeric_limits<double>::max();
                        for (const auto& instance: clusters[i]) {
                            dist = this->m->Distance(instance, clusterRepresentatives[i]);
                            if (dist < min) {
                                min = dist;
                                nearest = instance;
                            }
                        }
                        representatives.push_back(nearest);
                    } else {
                        representatives.push_back(clusterRepresentatives[i]);
                    }
                } else {
                    borders.push_back(this->FindBorders(clusters[i]));
                }
            }
        }

        for (const auto& rep: representatives) {
            temp.push_back(rep);
        }

        for (const auto& border: borders) {
            for (const auto& instance: border) {
                auto it = std::find(temp.begin(), temp.end(), instance);
                if (it == temp.end()) {
                    temp.push_back(instance);
                }
            }
        }

        selected = std::vector<Instance>(temp.begin(), temp.end());
    }
    return selected;
}

bool PSC::IsHomogeneous(const std::vector<Instance> &data) {
    if (!data.empty()) {
        int cx = data[0].GetClass();
        for (int i = 1; i < data.size(); i++) {
            if (data[i].GetClass() != cx) {
                return false;
            }
        }
    }
    return true;
}

std::vector<Instance>
PSC::FindBorders(const std::vector<Instance> &data) {
    std::list<Instance> select;
    std::vector<Instance> border;
    std::vector<std::vector<Instance>> clusters = Utils::SplitDataSetIntoClasses(data);
    Instance nearestc, nearestm;
    int cm = -1, max = 0;
    double min, dist;
    for (int i = 0; i < clusters.size(); i++) {
        if (clusters[i].size() > max) {
            max = clusters[i].size();
            cm = i;
        }
    }

    for (int i = 0; i < clusters.size(); i++) {
        if (i != cm && !clusters[i].empty()) {
            for (const auto& x: clusters[i]) {
                nearestc = Instance();
                min = std::numeric_limits<double>::max();
                for (const auto& y: clusters[cm]) {
                    dist = m->Distance(x, y);
                    if (dist < min) {
                        min = dist;
                        nearestc = y;
                    }
                }

                nearestm = Instance();
                min = std::numeric_limits<double>::max();
                for (const auto& y: clusters[i]) {
                    dist = m->Distance(nearestc, y);
                    if (dist < min) {
                        min = dist;
                        nearestm = y;
                    }
                }

                select.push_back(nearestc);
                select.push_back(nearestm);
            }
        }
    }

    border = std::vector<Instance>(select.begin(), select.end());
    return border;
}
