#include "Clustering.h"

const long Clustering::GetRuntime() {
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    long time = duration.count();
    return time;
}
