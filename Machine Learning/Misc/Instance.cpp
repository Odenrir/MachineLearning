#include "Instance.h"

Instance::Instance() {
    this->cNumericFeatures = 0;
    this->cCategoricFeatures = 0;
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = false;
    this->mixed = false;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::list<float> &feat) {
    this->numericFeatures = std::vector<float>(feat.begin(), feat.end());
    this->cNumericFeatures = (int) feat.size();
    this->cCategoricFeatures = 0;
    this->cLabels = 0;
    this->numerical = true;
    this->categorical = false;
    this->mixed = false;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::vector<float> &feat) {
    this->numericFeatures = feat;
    this->cNumericFeatures = (int) feat.size();
    this->cCategoricFeatures = 0;
    this->cLabels = 0;
    this->numerical = true;
    this->categorical = false;
    this->mixed = false;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::list<int> &feat) {
    this->categoricFeatures = std::vector<int>(feat.begin(), feat.end());
    this->cCategoricFeatures = (int) feat.size();
    this->cNumericFeatures = 0;
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = true;
    this->mixed = false;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::vector<int> &feat) {
    this->categoricFeatures = feat;
    this->cCategoricFeatures = (int) feat.size();
    this->cNumericFeatures = 0;
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = true;
    this->mixed = false;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::list<float> &numericFeat, const std::list<int> &categoricFeat) {
    this->numericFeatures = std::vector<float>(numericFeat.begin(), numericFeat.end());
    this->categoricFeatures = std::vector<int>(categoricFeat.begin(), categoricFeat.end());
    this->cNumericFeatures = (int) numericFeat.size();
    this->cCategoricFeatures = (int) categoricFeat.size();
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = false;
    this->mixed = true;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::vector<float> &numericFeat, const std::list<int> &categoricFeat) {
    this->numericFeatures = numericFeat;
    this->categoricFeatures = std::vector<int>(categoricFeat.begin(), categoricFeat.end());
    this->cNumericFeatures = (int) numericFeat.size();
    this->cCategoricFeatures = (int) categoricFeat.size();
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = false;
    this->mixed = true;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::list<float> &numericFeat, const std::vector<int> &categoricFeat) {
    this->numericFeatures = std::vector<float>(numericFeat.begin(), numericFeat.end());
    this->categoricFeatures = categoricFeat;
    this->cNumericFeatures = (int) numericFeat.size();
    this->cCategoricFeatures = (int) categoricFeat.size();
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = false;
    this->mixed = true;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

Instance::Instance(const std::vector<float> &numericFeat, const std::vector<int> &categoricFeat) {
    this->numericFeatures = numericFeat;
    this->categoricFeatures = categoricFeat;
    this->cNumericFeatures = (int) numericFeat.size();
    this->cCategoricFeatures = (int) categoricFeat.size();
    this->cLabels = 0;
    this->numerical = false;
    this->categorical = false;
    this->mixed = true;
    this->multiLabel = false;
    this->label = -1;
    this->id = -1;
}

std::vector<float> &Instance::GetNumericFeatures() {
    return this->numericFeatures;
}

float Instance::GetNumericFeature(int index) const {
    return this->numericFeatures[index];
}

std::vector<int> &Instance::GetCategoricFeatures() {
    return this->categoricFeatures;
}

int Instance::GetCategoricFeature(int index) const {
    return this->categoricFeatures[index];
}

bool Instance::IsMultilabel() const {
    return this->multiLabel;
}

bool Instance::IsNumeric() const {
    return this->numerical;
}

bool Instance::IsNominal() const {
    return this->categorical;
}

bool Instance::IsMixed() const {
    return this->mixed;
}

std::list<int> &Instance::GetClasses() {
    return this->labels;
}

std::vector<int> Instance::GetDescriptor() const {
    return this->featDescriptor;
}

int Instance::CountNumericFeatures() const {
    return this->cNumericFeatures;
}

int Instance::CountCategoricFeatures() const {
    return this->cCategoricFeatures;
}

int Instance::CountFeatures() const {
    return this->cNumericFeatures + this->cCategoricFeatures;
}

int Instance::CountClasses() const {
    return this->cLabels;
}

std::string Instance::GetName() const {
    return this->name;
}

std::string Instance::ToString() const {
    std::string result;
    if (this->cNumericFeatures > 0) {
        result += " Numeric: ";
        for (int i = 0; i < this->cNumericFeatures; i++) {
            result += std::to_string(this->numericFeatures[i]);
            result += " ";
        }
    }
    if (this->cCategoricFeatures > 0) {
        result += " Nominal: ";
        for (int i = 0; i < this->cCategoricFeatures; i++) {
            result += std::to_string(this->categoricFeatures[i]);
            result += " ";
        }
    }
    result += " --- Class: " + std::to_string(this->label);
    result += "\n";
    return result;
}

int Instance::GetClass() const {
    return this->label;
}

int Instance::GetID() const {
    return this->id;
}

void Instance::SetDescriptor(const std::list<int> &d) {
    this->featDescriptor = std::vector<int>(d.begin(), d.end());
}

void Instance::SetDescriptor(const std::vector<int> &d) {
    this->featDescriptor = d;
}

void Instance::SetClass(int c) {
    this->label = c;
    this->cLabels = 1;
    this->multiLabel = false;
}

void Instance::SetClasses(const std::list<int> &c) {
    this->labels = c;
    this->cLabels = (int) c.size();
    this->multiLabel = true;
}

void Instance::SetID(int ID) {
    this->id = ID;
}

void Instance::SetName(const std::string &n) {
    this->name = n;
}
