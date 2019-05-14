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
