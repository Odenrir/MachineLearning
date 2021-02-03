/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#pragma once

#include <chrono>
#include "Misc/Instance.h"

class Classifier {
public:
    virtual void Init() = 0;

    virtual void Learn(const std::vector<Instance> &train) = 0;

    virtual int Classify(const Instance &inst) = 0;

    virtual const std::vector<int> Classify(const std::vector<Instance> &test) = 0;

    const long GetLearningRuntime();

    const long GetSingleClassifyingRuntime();

    const long GetMultiClassifyingRuntime();

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> startLearning, stopLearning, startClassifyingSingle, stopClassifyingSingle, startClassifyingMulti, stopClassifyingMulti;
};