/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#pragma once
#include "Misc/Instance.h"
#include <cmath>

class Metric {
public:

    virtual void Init() = 0;

    virtual double Distance(const Instance &inst1, const Instance &inst2) = 0;

};