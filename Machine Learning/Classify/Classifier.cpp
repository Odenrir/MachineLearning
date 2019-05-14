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
