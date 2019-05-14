#include <utility>

#include "kFoldCrossValidation.h"

kFoldCrossValidation::kFoldCrossValidation() {
    this->k = 0;
}

kFoldCrossValidation::kFoldCrossValidation(int k, const std::vector<Instance> &train, Classifier &c) {
    this->train = train;
    this->k = k;
    this->SplitStratified();
    //this->Split();
    this->cl = &c;
}

void kFoldCrossValidation::Init() {
}

double kFoldCrossValidation::Validate() {
    double acc = 0;
    float temp;
    std::vector<float> accs(this->k);
    std::vector<int> predict;
    for (int i = 0; i < this->k; i++) {
        std::cout << "Learning Fold " << i + 1 << "\n";
        this->cl->Learn(this->trainFolds[i]);
        std::cout << "Classifying Fold " << i + 1 << "\n";
        predict = this->cl->Classify(this->testFolds[i]);
        temp = Utils::Accuraccy(this->testFolds[i], predict);
        acc += temp;
        accs[i] = temp;
        std::cout << "acc " << i + 1 << ": " << temp << "\n";
    }
    acc = acc / this->k;
    return acc;
}

std::vector<float> kFoldCrossValidation::ValidateIS(InstanceSelection &algorithm, std::string fileName) {
    std::vector<float> acc(2, 0);
    float temp1, temp2;
    std::vector<Instance> selected;
    std::vector<float> accs(this->k);
    std::vector<float> accsIS(this->k);
    std::vector<float> ret(this->k);
    std::vector<long> runtime(this->k);
    std::vector<int> predict;
    for (int i = 0; i < this->k; i++) {
        std::cout << "Learning Fold " << i + 1 << "\n";
        this->cl->Learn(this->trainFolds[i]);
        std::cout << "Classifying Fold " << i + 1 << "\n";
        predict = this->cl->Classify(this->testFolds[i]);
        temp1 = Utils::Accuraccy(this->testFolds[i], predict);
        acc[0] += temp1;
        accs[i] = temp1;
        std::cout << "Instance Selection Fold " << i + 1 << "\n";
        std::vector<Instance>().swap(selected);
        selected = algorithm.DoSelection(this->trainFolds[i]);
        std::cout << "Learning IS Fold " << i + 1 << "\n";
        this->cl->Learn(selected);
        predict = this->cl->Classify(this->testFolds[i]);
        temp2 = Utils::Accuraccy(this->testFolds[i], predict);
        acc[1] += temp2;
        accsIS[i] = temp2;
        ret[i] = ((float) selected.size() / (float) this->trainFolds[i].size()) * 100;
        runtime[i] = algorithm.GetRuntime();
        algorithm.Clear();
    }
    this->WriteFile(fileName, accs, accsIS, ret, runtime);
    acc[0] = acc[0] / this->k;
    acc[1] = acc[1] / this->k;
    return acc;
}

void kFoldCrossValidation::Split() {
    int ts = (((int) this->train.size()) / this->k);
    int tr = (int) this->train.size() - ts;
    int count = 0;
    this->trainFolds = std::vector<std::vector<Instance>>(this->k);
    this->testFolds = std::vector<std::vector<Instance>>(this->k);

    for (int i = 0; i < this->k; i++) {
        this->testFolds[i] = std::vector<Instance>(ts);
        for (int j = 0; j < ts; j++) {
            this->testFolds[i][j] = this->train[count];
            count++;
        }
    }

    for (int i = 0; i < this->k; i++) {
        count = 0;
        this->trainFolds[i] = std::vector<Instance>(tr);
        for (int j = 0; j < train.size(); j++) {
            if (j < ts * i || j >= ts * (i + 1)) {
                this->trainFolds[i][count] = this->train[j];
                count++;
            }
        }
    }
}

void kFoldCrossValidation::SplitStratified() {
    std::vector<std::vector<int>> split = Utils::SplitDataSetIntoClassesIndices(this->train);
    int ts = (((int) this->train.size()) / this->k);
    std::vector<int> count(split.size(), 0);
    this->trainFolds = std::vector<std::vector<Instance>>(this->k);
    this->testFolds = std::vector<std::vector<Instance>>(this->k);
    std::vector<int> tsv = std::vector<int>(split.size());
    std::vector<int> trv = std::vector<int>(split.size());
    for (int i = 0; i < split.size(); i++) {
        if (split[i].size() < this->k) {
            tsv[i] = 1;
        } else {
            tsv[i] = (((int) split[i].size()) / this->k);
        }
        trv[i] = split[i].size() - tsv[i];
    }
    auto trainAux = std::vector<std::list<Instance>>(this->k);
    auto testAux = std::vector<std::list<Instance>>(this->k);

    for (int i = 0; i < this->k; i++) {
        std::list<Instance> test;
        for (int j = 0; j < split.size(); j++) {
            if (count[j] < split[j].size()) {
                for (int l = 0; l < tsv[j]; l++) {
                    test.push_back(this->train[split[j][count[j]]]);
                    count[j]++;
                }
            }
        }
        this->testFolds[i] = std::vector<Instance>(test.begin(), test.end());
    }

    for (int i = 0; i < this->k; i++) {
        std::list<Instance> training;
        for (int j = 0; j < split.size(); j++) {
            for (int l = 0; l < trv[j]; l++) {
                if (l < tsv[j] * i || l >= tsv[j] * (i + 1)) {
                    training.push_back(this->train[split[j][l]]);
                }
            }
        }
        this->trainFolds[i] = std::vector<Instance>(training.begin(), training.end());
    }

}

void kFoldCrossValidation::WriteFile(const std::string &fileName, std::vector<float> acc, std::vector<float> accIS,
                                     std::vector<float> ret, std::vector<long> runtimes) {
    std::string text;
    std::ofstream result;
    result.open(fileName);
    float avgOrig = 0, avgIS = 0, avgRet = 0;
    double avgRun = 0;
    text = "Fold\t\tAccuraccy Original\t\t\tAccuraccy IS\t\t\tRetention\t\t\tRuntime IS\n";
    for (int i = 0; i < acc.size(); i++) {
        text += std::to_string(i + 1) + "\t\t\t\t" + std::to_string(acc[i]) + "\t\t\t\t" + std::to_string(accIS[i]) +
                "\t\t\t\t" + std::to_string(ret[i]) + "\t\t\t" + std::to_string(runtimes[i]) + "\n";
        avgOrig += acc[i];
        avgIS += accIS[i];
        avgRet += ret[i];
        avgRun += runtimes[i];
    }
    text += "\nAvg\t\t\t\t" + std::to_string(avgOrig / acc.size()) + "\t\t\t\t" + std::to_string(avgIS / acc.size()) +
            "\t\t\t\t" + std::to_string(avgRet / acc.size()) + "\t\t\t" + std::to_string(avgRun / acc.size()) + "\n";
    result << text;
    result.close();
    std::cout << text;
}
