/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include <cmath>
#include "HEOM.h"


HEOM::HEOM() {
}

void HEOM::Init() {
}

double HEOM::Distance(const Instance &inst1, const Instance &inst2) {
    double result = 0.0, aux;

    for (int i = 0; i < inst1.CountNumericFeatures(); i++) {
        aux = pow((inst1.GetNumericFeature(i) - inst2.GetNumericFeature(i)) / this->ranges[i], 2);
        if (aux == std::numeric_limits<double>::infinity()) {
            aux = 1;
        }
        result += aux;
    }
    for (int i = 0; i < inst1.CountCategoricFeatures(); i++) {
        if (inst1.GetCategoricFeature(i) == -1 || inst2.GetCategoricFeature(i) == -1) {
            aux = 1;
        } else if (inst1.GetCategoricFeature(i) == inst2.GetCategoricFeature(i)) {
            aux = 0;
        } else {
            aux = 1;
        }
        result += aux;
    }
    result = sqrt(result) / sqrt(inst1.CountFeatures());
    return result;
}

//Must be used before any call to distance function
void HEOM::Normalize(const std::vector<Instance> &dataset) {
    if (!dataset.empty()) {
        int featNumber = dataset[0].CountNumericFeatures();
        std::vector<double>().swap(this->ranges);
        this->ranges = std::vector<double>(featNumber);
        float max, min;
        for (int i = 0; i < featNumber; i++) {
            max = std::numeric_limits<float>::min();
            min = std::numeric_limits<float>::max();
            for (const auto &instance: dataset) {
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
            this->ranges[i] = std::fabs(max - min);
            if (this->ranges[i] == 0) {
                this->ranges[i] = 1;
            }
        }
    }
}
