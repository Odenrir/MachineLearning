#ifndef MACHINELEARNING_INSTANCESELECTION_H
#define MACHINELEARNING_INSTANCESELECTION_H
#include "Misc/Instance.h"

class InstanceSelection {
public:
    virtual void Init() = 0;

    virtual std::vector<Instance> DoSelection(const std::vector<Instance> &dataset) = 0;


};
#endif //MACHINELEARNING_INSTANCESELECTION_H
