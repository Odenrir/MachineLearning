#include "KFoldCrossValidation.h"

KFoldCrossValidation::KFoldCrossValidation() {
}

KFoldCrossValidation::KFoldCrossValidation(int k, const std::vector<Instance> &train, Classifier &c) {
    this->train = train;
    this->k = k;
    this->Split();
    this->cl = &c;
}

void KFoldCrossValidation::Init() {
}

double KFoldCrossValidation::Validate() {
    double acc = 0;
    float temp;
    std::vector<float> accs(this->k);
    std::vector<int> predict;
    for (int i = 0; i < this->k; i++) {
        std::cout << "Learning Fold " << i << "\n";
        this->cl->Learn(this->trainFolds[i]);
        std::cout << "Classifying Fold " << i << "\n";
        predict = this->cl->Classify(this->testFolds[i]);
        temp = Utils::Accuraccy(this->testFolds[i], predict);
        acc += temp;
        accs[i] = temp;
        std::cout << "acc " << i << ": " << temp << "\n";
    }
    acc = acc / this->k;
    return acc;
}

void KFoldCrossValidation::Split() {
    int ts = (((int) this->train.size()) / this->k);
    int tr = (int) this->train.size() - ts;
    int res = tr - (ts * this->k);
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
