/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#ifndef MACHINELEARNING_INSTANCESELECTION_H
#define MACHINELEARNING_INSTANCESELECTION_H

#include <chrono>
#include "Misc/Instance.h"

class InstanceSelection {
public:
    virtual void Init() = 0;

    virtual void Clear() = 0;

    virtual std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) = 0;

    const long GetRuntime();

protected:
    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
};

#endif //MACHINELEARNING_INSTANCESELECTION_H
