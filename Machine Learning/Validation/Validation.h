/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#pragma once
#include "Classify/Classifier.h"

class Validation {
public:
    virtual void Init() = 0;

    virtual double Validate() = 0;

protected:
    Classifier *cl;
};