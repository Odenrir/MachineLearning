/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "InstanceSelection.h"

const long InstanceSelection::GetRuntime() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    long time = duration.count();
    return time;
}
