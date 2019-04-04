#pragma once

#include "Instance.h"
#include <cmath>

class Metric {
public:

    virtual void Init() = 0;

    virtual double Distance(const Instance &inst1, const Instance &inst2) = 0;

};