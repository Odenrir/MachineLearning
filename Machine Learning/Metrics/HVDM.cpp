#include <cmath>

#include "HVDM.h"


HVDM::HVDM() {
    this->labels = 0;
}

void HVDM::Init() {
}

double HVDM::Distance(const Instance &inst1, const Instance &inst2) {
    double result = 0.0, aux;

    for (int i = 0; i < inst1.CountNumericFeatures(); i++) {
        if (inst1.GetNumericFeature(i) != std::numeric_limits<float>::infinity() &&
            inst2.GetNumericFeature(i) != std::numeric_limits<float>::infinity()) {
            aux = pow(this->normalizedDiff(i, inst1.GetNumericFeature(i), inst2.GetNumericFeature(i)), 2);
            if (aux == std::numeric_limits<double>::infinity()) {
                aux = 1;
            }
        } else {
            aux = 1;
        }
        result += aux;
    }
    for (int i = 0; i < inst1.CountCategoricFeatures(); i++) {
        if (inst1.GetCategoricFeature(i) == -1 || inst2.GetCategoricFeature(i) == -1) {
            aux = 1;
        } else {
            aux = pow(this->VDM(i, inst1.GetCategoricFeature(i), inst2.GetCategoricFeature(i)), 2);
        }
        result += aux;
    }
    sqrt(result) / sqrt(inst1.CountFeatures());
    return result;
}

double HVDM::VDM(int index, int x, int y) {
    int naxi, nayi, naxci, nayci;
    if (this->na[index].count(x) > 0 && this->na[index].count(y) > 0) {
        naxi = this->na[index][x];
        nayi = this->na[index][y];
    } else {
        return 1;
    }

    double result = 0;

    for (int i = 0; i < this->labels; i++) {
        if (this->naC[index].count(x) > 0 && this->naC[index][x].count(i) > 0) {
            naxci = this->naC[index][x][i];
        } else {
            naxci = 0;
        }
        if (this->naC[index].count(y) > 0 && this->naC[index][y].count(i) > 0) {
            nayci = this->naC[index][y][i];
        } else {
            nayci = 0;
        }
        result += pow(std::fabs(((float) naxci / naxi) - ((float) nayci / nayi)), 2);
    }
    return sqrt(result);// / sqrt(2);//sqrt(this->labels);
}

double HVDM::normalizedDiff(int index, float x, float y) {
    if (this->stdDev4[index] != 0) {
        return fabsf(x - y) / this->stdDev4[index];
    }
    return 0;
}

//Must be used before any call to distance function
void HVDM::Normalize(const std::vector<Instance> &dataset) {
    if (!dataset.empty()) {
        this->ComputeStandardDeviation(dataset);
        this->ComputeOccurrences(dataset);
        std::vector<int> descriptor(dataset[0].GetDescriptor());
        this->labels = descriptor[descriptor.size() - 1];
    }
}

void HVDM::ComputeOccurrences(const std::vector<Instance> &dataset) {
    if (!dataset.empty()) {
        int featNumber = dataset[0].CountCategoricFeatures();
        this->na.clear();
        this->naC.clear();
        for (int i = 0; i < featNumber; i++) {
            for (const auto& instance: dataset) {
                int value = instance.GetCategoricFeature(i);
                if (value != -1) {
                    if (this->na[i].count(value) > 0) {
                        this->na[i][value]++;
                    } else {
                        this->na[i][value] = 1;
                    }
                }
            }
            for (const auto& instance: dataset) {
                int value = instance.GetCategoricFeature(i);
                if (value != -1) {
                    int category = instance.GetClass();
                    if (this->naC[i].count(value) > 0 && this->naC[i][value].count(category) > 0) {
                        this->naC[i][value][category]++;
                    } else {
                        this->naC[i][value][category] = 1;
                    }
                }
            }
        }
    }
}

void HVDM::ComputeStandardDeviation(const std::vector<Instance> &dataset) {
    double mean, squaresMean, meanSquare;
    int n;
    if (!dataset.empty()) {
        int featNumber = dataset[0].CountNumericFeatures();
        std::vector<double>().swap(this->stdDev4);
        this->stdDev4 = std::vector<double>(featNumber);
        for (int i = 0; i < featNumber; i++) {
            mean = 0, squaresMean = 0, n = 0;
            for (const auto& instance : dataset) {
                float value = instance.GetNumericFeature(i);
                if (value != std::numeric_limits<float>::infinity()) {
                    mean += value;
                    squaresMean += pow(value, 2);
                    n++;
                }
            }
            if (n > 0) {
                mean = mean / n;
                meanSquare = pow(mean, 2);
                squaresMean = squaresMean / n;
                this->stdDev4[i] = 4 * sqrt(squaresMean - meanSquare);
            } else {
                this->stdDev4[i] = 0;
            }
        }
    }
}


