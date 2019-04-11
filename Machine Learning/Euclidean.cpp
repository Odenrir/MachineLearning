#include "Euclidean.h"

Euclidean::Euclidean() {
}

void Euclidean::Init() {
}

double Euclidean::Distance(const Instance &inst1, const Instance &inst2) {
    double result = 0.0, aux;

    for (int i = 0; i < inst1.CountNumericFeatures(); i++) {
        aux = pow((inst1.GetNumericFeature(i) - inst2.GetNumericFeature(i)) / this->ranges[i], 2);
        if (aux == std::numeric_limits<double>::infinity()) {
            aux = 1;
        }
        result += aux;
    }
    sqrt(result) / sqrt(inst1.CountNumericFeatures());
    return result;
}

//Must be used before any call to distance function
void Euclidean::Normalize(const std::vector<Instance> &train) {
    if (!train.empty()) {
        int featNumber = train[0].CountNumericFeatures();
        std::vector<double>().swap(this->ranges);
        this->ranges = std::vector<double>(featNumber);
        float max, min;
        for (int i = 0; i < featNumber; i++) {
            max = std::numeric_limits<float>::min();
            min = std::numeric_limits<float>::max();
            for (auto instance: train) {
                float value = instance.GetNumericFeature(i);
                if (value != std::numeric_limits<float>::infinity()) {
                    if (value < min) {
                        min = value;
                    }
                    if (value > max) {
                        max = value;
                    }
                }
            }
            this->ranges[i] = fabs(max - min);
            if (this->ranges[i] == 0) {
                this->ranges[i] = 1;
            }
        }
    }
}
