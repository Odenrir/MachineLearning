#pragma once

#include "Classifier.h"

class Validation {
public:
    virtual void Init() = 0;

    virtual double Validate() = 0;

protected:
    Classifier *cl;
};