
#include "Plot.h"

Plot::Plot() {}

void Plot::PlotScatter2D(const std::vector<Instance> &dataset) {
    if (!dataset.empty() && dataset[0].CountNumericFeatures() > 1) {
        std::vector<std::vector<int>> clusters = Utils::SplitDataSetIntoClassesIndices(dataset);
        std::vector<std::vector<double>> x(clusters.size());
        std::vector<std::vector<double>> y(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            x[i] = std::vector<double>(clusters[i].size());
            y[i] = std::vector<double>(clusters[i].size());
            for (int j = 0; j < clusters[i].size(); j++) {
                x[i][j] = dataset[clusters[i][j]].GetNumericFeature(0);
                y[i][j] = dataset[clusters[i][j]].GetNumericFeature(1);
            }
        }
        Scatter(x, y);
    }
}

void Plot::PlotScatter2D(const std::vector<std::vector<Instance>> &clusters) {
    if (!clusters.empty()) {
        std::vector<std::vector<double>> x(clusters.size());
        std::vector<std::vector<double>> y(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            x[i] = std::vector<double>(clusters[i].size());
            y[i] = std::vector<double>(clusters[i].size());
            for (int j = 0; j < clusters[i].size(); j++) {
                x[i][j] = clusters[i][j].GetNumericFeature(0);
                y[i][j] = clusters[i][j].GetNumericFeature(1);
            }
        }
        Scatter(x, y);
    }
}

void Plot::ScalePlotScatter2D(const std::vector<Instance> &reference, const std::vector<Instance> &dataset) {
    if (!reference.empty() && !dataset.empty() && reference[0].CountNumericFeatures() > 1) {
        std::vector<double> x1(reference.size());
        std::vector<double> y1(reference.size());
        for (int i = 0; i < reference.size(); i++) {
            x1[i] = reference[i].GetNumericFeature(0);
            y1[i] = reference[i].GetNumericFeature(1);
        }
        double maxX = *std::max_element(x1.begin(), x1.end());
        double minX = *std::min_element(x1.begin(), x1.end());
        double maxY = *std::max_element(y1.begin(), y1.end());
        double minY = *std::min_element(y1.begin(), y1.end());
        std::vector<std::vector<int>> clusters = Utils::SplitDataSetIntoClassesIndices(dataset);
        std::vector<std::vector<double>> x(clusters.size());
        std::vector<std::vector<double>> y(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            x[i] = std::vector<double>(clusters[i].size());
            y[i] = std::vector<double>(clusters[i].size());
            for (int j = 0; j < clusters[i].size(); j++) {
                x[i][j] = dataset[clusters[i][j]].GetNumericFeature(0);
                y[i][j] = dataset[clusters[i][j]].GetNumericFeature(1);
            }
        }
        ScatterLimit(x, y, minX, maxX, minY, maxY);
    }

}

void Plot::ScalePlotScatter2D(const std::vector<std::vector<Instance>> &reference,
                              const std::vector<std::vector<Instance>> &clusters) {
    if (!reference.empty() && !clusters.empty()) {
        std::list<Instance> aux;
        for (const auto& category : reference) {
            for (const auto & instance : category) {
                aux.push_back(instance);
            }
        }
        std::vector<Instance> auxReference = std::vector<Instance>(aux.begin(), aux.end());
        std::vector<double> x1(auxReference.size());
        std::vector<double> y1(auxReference.size());
        for (int i = 0; i < auxReference.size(); i++) {
            x1[i] = auxReference[i].GetNumericFeature(0);
            y1[i] = auxReference[i].GetNumericFeature(1);
        }
        double maxX = *std::max_element(x1.begin(), x1.end());
        double minX = *std::min_element(x1.begin(), x1.end());
        double maxY = *std::max_element(y1.begin(), y1.end());
        double minY = *std::min_element(y1.begin(), y1.end());

        std::vector<std::vector<double>> x(clusters.size());
        std::vector<std::vector<double>> y(clusters.size());
        for (int i = 0; i < clusters.size(); i++) {
            x[i] = std::vector<double>(clusters[i].size());
            y[i] = std::vector<double>(clusters[i].size());
            for (int j = 0; j < clusters[i].size(); j++) {
                x[i][j] = clusters[i][j].GetNumericFeature(0);
                y[i][j] = clusters[i][j].GetNumericFeature(1);
            }
        }
        ScatterLimit(x, y, minX, maxX, minY, maxY);
    }
}

void Plot::Scatter(const std::vector<std::vector<double>>& x, const std::vector<std::vector<double>>& y) {
    plt::figure_size(1200, 780);
    for (int i = 0; i < x.size(); i++) {
        plt::plot(x[i], y[i], "o");
    }
    plt::show();
}

void
Plot::ScatterLimit(const std::vector<std::vector<double>>& x, const std::vector<std::vector<double>>& y, double xMin,
                   double xMax, double yMin, double yMax) {
    plt::figure_size(1200, 780);
    plt::xlim(xMin, xMax);
    plt::ylim(yMin, yMax);
    for (int i = 0; i < x.size(); i++) {
        plt::plot(x[i], y[i], "o");
    }
    plt::show();

}

