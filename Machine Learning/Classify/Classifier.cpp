/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "Classifier.h"

const long Classifier::GetLearningRuntime() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopLearning - startLearning);
    long time = duration.count();
    return time;
}

const long Classifier::GetSingleClassifyingRuntime() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            stopClassifyingSingle - startClassifyingSingle);
    long time = duration.count();
    return time;
}

const long Classifier::GetMultiClassifyingRuntime() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stopClassifyingMulti - startClassifyingMulti);
    long time = duration.count();
    return time;
}
