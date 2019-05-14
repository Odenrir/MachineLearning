#pragma once

#include <cstdlib>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>
#include <string>

class Instance {
public:
    bool operator==(const Instance &inst) {
        int numFeatSize = this->CountNumericFeatures();
        int catFeatSize = this->CountCategoricFeatures();

        for (int i = 0; i < numFeatSize; i++) {
            if (this->GetNumericFeature(i) != inst.GetNumericFeature(i))
                return false;
        }
        for (int i = 0; i < catFeatSize; i++) {
            if (this->GetCategoricFeature(i) != inst.GetCategoricFeature(i))
                return false;
        }
        return true;
    }

    Instance();

    explicit Instance(const std::list<float> &feat);

    explicit Instance(const std::vector<float> &feat);

    explicit Instance(const std::list<int> &feat);

    explicit Instance(const std::vector<int> &feat);

    Instance(const std::list<float> &numericFeat, const std::list<int> &categoricFeat);

    Instance(const std::vector<float> &numericFeat, const std::list<int> &categoricFeat);

    Instance(const std::list<float> &numericFeat, const std::vector<int> &categoricFeat);

    Instance(const std::vector<float> &numericFeat, const std::vector<int> &categoricFeat);

    std::vector<float> &GetNumericFeatures();

    std::vector<int> &GetCategoricFeatures();

    float GetNumericFeature(int index) const;

    int GetCategoricFeature(int index) const;

    bool IsMultilabel() const;

    bool IsNumeric() const;

    bool IsNominal() const;

    bool IsMixed() const;

    std::list<int> &GetClasses();

    std::vector<int> GetDescriptor() const;

    int GetClass() const;

    int GetID() const;

    int CountNumericFeatures() const;

    int CountCategoricFeatures() const;

    int CountFeatures() const;

    int CountClasses() const;

    std::string GetName() const;

    std::string ToString() const;

    void SetDescriptor(const std::list<int> &d);

    void SetDescriptor(const std::vector<int> &d);

    void SetClass(int c);

    void SetClasses(const std::list<int> &c);

    void SetID(int ID);

    void SetName(const std::string &n);

private:
    bool multiLabel;
    std::vector<float> numericFeatures;
    std::vector<int> categoricFeatures;
    std::vector<int> featDescriptor;
    std::list<int> labels;
    int label;
    int cNumericFeatures;
    int cCategoricFeatures;
    int cLabels;
    int id;
    bool mixed;
    bool numerical;
    bool categorical;
    std::string name;

};