#ifndef MACHINELEARNING_PLOT_H
#define MACHINELEARNING_PLOT_H

#include "Instance.h"
#include "Misc/Utils.h"
#include <string>
#include "mathplotlibcpp.h"

namespace plt = matplotlibcpp;

class Plot {
public:
    Plot();

    static void PlotScatter2D(const std::vector<Instance> &dataset);

    static void PlotScatter2D(const std::vector<std::vector<Instance>> &clusters);

    static void ScalePlotScatter2D(const std::vector<Instance> &reference, const std::vector<Instance> &dataset);

    static void ScalePlotScatter2D(const std::vector<std::vector<Instance>> &reference,
                                   const std::vector<std::vector<Instance>> &clusters);

private:
    static void Scatter(const std::vector<std::vector<double>> &x, const std::vector<std::vector<double>> &y);

    static void ScatterLimit(const std::vector<std::vector<double>> &x, const std::vector<std::vector<double>> &y,
                             double xMin, double xMax, double yMin, double yMax);
};


#endif //MACHINELEARNING_PLOT_H
