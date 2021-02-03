/* Copyright (C) Argenis Aldair Aroche Villarruel, Inc - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Argenis Aldair Aroche Villarruel <argenis_aroche@hotmail.com>, 2018
 */
#include "Utils.h"

Utils::Utils() {
}

std::vector<Instance> Utils::ReadARFF(const std::string &training) {
    std::list<Instance> result;
    Instance inst;
    std::list<std::map<std::string, int>> listMapNominal;
    std::vector<std::map<std::string, int>> vectorMapNominal;
    std::list<int> descriptorList;
    std::vector<int> descriptorVector;
    std::vector<std::string> splitString;
    std::vector<std::string> splitNominal;
    std::ifstream file(training);
    std::string line, aux;
    bool dataReading = false;
    std::string description("%");
    std::string attribute("@ATTRIBUTE");
    std::string data("@DATA");
    std::string numeric("NUMERIC");
    std::string real("REAL");
    std::string integer("INTEGER");
    std::string missing("?");
    std::string empty;
    int numericValues = 0, nominalValues = 0, id = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!dataReading && StartsWithCaseInsensitive(line, attribute)) {
                line = std::regex_replace(line, std::regex("\\s{2,}"), " ");
                std::replace(line.begin(), line.end(), '\t', ' ');
                boost::split(splitString, line, boost::is_any_of(" "));

                if (CaseInsensitiveStringCompare(splitString[2], numeric) ||
                    CaseInsensitiveStringCompare(splitString[2], real) ||
                    CaseInsensitiveStringCompare(splitString[2], integer)) {
                    descriptorList.push_back(0);
                } else {
                    std::map<std::string, int> mapNominal;
                    if (splitString.size() > 3) {
                        for (int i = 3; i < splitString.size(); i++) {
                            splitString[2] += splitString[i];
                        }
                    }
                    aux = boost::algorithm::trim_copy(splitString[2].substr(1, splitString[2].size() - 2));
                    boost::split(splitNominal, aux, boost::is_any_of(","));
                    descriptorList.push_back(splitNominal.size());
                    for (int i = 0; i < splitNominal.size(); i++) {
                        mapNominal[splitNominal[i]] = i;
                    }
                    listMapNominal.push_back(mapNominal);
                }

            } else if (StartsWithCaseInsensitive(line, data)) {
                dataReading = true;
                descriptorVector = std::vector<int>(descriptorList.begin(), descriptorList.end());
                vectorMapNominal = std::vector<std::map<std::string, int>>(listMapNominal.begin(),
                                                                           listMapNominal.end());
                for (auto x : descriptorVector) {
                    if (x == 0) {
                        numericValues++;
                    } else {
                        nominalValues++;
                    }
                }
                nominalValues--;
            } else if (dataReading && !StartsWithCaseInsensitive(line, description) &&
                       !CaseInsensitiveStringCompare(line, empty)) {
                std::vector<float> numericFeatures(numericValues);
                std::vector<int> nominalFeatures(nominalValues);
                int j = 0, k = 0;
                boost::split(splitString, line, boost::is_any_of(","));
                for (int i = 0; i < descriptorVector.size() - 1; i++) {
                    if (descriptorVector[i] == 0) {
                        std::string featAux(boost::algorithm::trim_copy(splitString[i]));
                        if (CaseInsensitiveStringCompare(featAux, missing)) {
                            numericFeatures[j] = std::numeric_limits<float>::infinity();
                        } else {
                            numericFeatures[j] = (float) atof(featAux.c_str());
                        }
                        j++;
                    } else {
                        if (vectorMapNominal[k].count(splitString[i]) > 0) {
                            nominalFeatures[k] = vectorMapNominal[k][splitString[i]];
                        } else {
                            nominalFeatures[k] = -1;
                        }
                        k++;
                    }
                }
                if (nominalValues == 0) {
                    inst = Instance(numericFeatures);
                } else if (numericValues == 0) {
                    inst = Instance(nominalFeatures);
                } else {
                    inst = Instance(numericFeatures, nominalFeatures);
                }
                if (vectorMapNominal[k].count(boost::algorithm::trim_copy(splitString[descriptorVector.size() - 1])) >
                    0) {
                    inst.SetID(id);
                    inst.SetClass(
                            vectorMapNominal[k][boost::algorithm::trim_copy(splitString[descriptorVector.size() - 1])]);
                    inst.SetDescriptor(descriptorVector);
                    id++;
                    result.push_back(inst);
                }
            }
        }

    }
    std::vector<Instance> vecRes{std::make_move_iterator(std::begin(result)),
                                 std::make_move_iterator(std::end(result))};
    return vecRes;
}

std::vector<Instance> Utils::ReadCSV(const std::string &training) {
    std::list<Instance> result;
    Instance inst;
    std::vector<std::map<std::string, int>> vectorMapNominal;
    std::list<int> descriptorList;
    std::vector<int> descriptorVector;
    std::vector<std::string> splitString;
    std::vector<std::string> splitNominal;
    std::ifstream file(training);
    std::string line, aux;
    bool dataReading = false;
    std::string missing("?");
    std::string empty;
    int numericValues = 0, nominalValues = 0, id = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!dataReading && !CaseInsensitiveStringCompare(line, empty)) {
                boost::split(splitString, line, boost::is_any_of(","));

                for (const std::string &headFeature: splitString) {
                    descriptorList.push_back(atoi(boost::algorithm::trim_copy(headFeature).c_str()));
                }
                dataReading = true;
                descriptorVector = std::vector<int>(descriptorList.begin(), descriptorList.end());
                for (auto x : descriptorVector) {
                    if (x == 0) {
                        numericValues++;
                    } else {
                        nominalValues++;
                    }
                }
                vectorMapNominal = std::vector<std::map<std::string, int>>(nominalValues);
                nominalValues--;
            } else if (dataReading && !CaseInsensitiveStringCompare(line, empty)) {
                std::vector<float> numericFeatures(numericValues);
                std::vector<int> nominalFeatures(nominalValues);
                int j = 0, k = 0;
                boost::split(splitString, line, boost::is_any_of(","));
                for (int i = 0; i < descriptorVector.size() - 1; i++) {
                    if (descriptorVector[i] == 0) {
                        std::string featAux(boost::algorithm::trim_copy(splitString[i]));
                        if (CaseInsensitiveStringCompare(featAux, missing)) {
                            numericFeatures[j] = std::numeric_limits<float>::infinity();
                        } else {
                            numericFeatures[j] = (float) atof(featAux.c_str());
                        }
                        j++;
                    } else {
                        if (vectorMapNominal[k].count(splitString[i]) > 0) {
                            nominalFeatures[k] = vectorMapNominal[k][splitString[i]];
                        } else if (!CaseInsensitiveStringCompare(splitString[k], missing)) {
                            vectorMapNominal[k][splitString[i]] = vectorMapNominal[k].size();
                            nominalFeatures[k] = vectorMapNominal[k][splitString[i]];
                        } else {
                            nominalFeatures[k] = -1;
                        }
                        k++;
                    }
                }
                if (nominalValues == 0) {
                    inst = Instance(numericFeatures);
                } else if (numericValues == 0) {
                    inst = Instance(nominalFeatures);
                } else {
                    inst = Instance(numericFeatures, nominalFeatures);
                }
                inst.SetID(id);
                aux = boost::algorithm::trim_copy(splitString[descriptorVector.size() - 1]);
                if (vectorMapNominal[k].count(aux) > 0) {
                    inst.SetClass(vectorMapNominal[k][aux]);
                } else {
                    vectorMapNominal[k][aux] = vectorMapNominal[k].size();
                    inst.SetClass(vectorMapNominal[k][aux]);
                }
                inst.SetDescriptor(descriptorVector);
                id++;
                result.push_back(inst);
            }
        }

    }
    std::vector<Instance> vecRes{std::make_move_iterator(std::begin(result)),
                                 std::make_move_iterator(std::end(result))};
    return vecRes;
}

bool Utils::CompareInstances(const std::vector<Instance> &inst1, const std::vector<Instance> &inst2) {
    if (inst1.size() != inst2.size())
        return false;

    std::vector<Instance> t1 = inst1;
    std::vector<Instance> t2 = inst2;

    for (int i = 0; i < inst1.size(); i++) {
        if (!(t1[i] == t2[i]))
            return false;
    }
    return true;
}

int Utils::Max(const std::vector<int> &c) {
    int index = -1, maxV = std::numeric_limits<int>::min(), i;
    for (i = 0; i < c.size(); i++) {
        if (c[i] > maxV) {
            maxV = c[i];
            index = i;
        }
    }
    return index;
}

int Utils::SameClass(int c, const std::vector<std::vector<Instance>> &categories) {
    for (int i = 0; i < categories.size(); i++) {
        if (c == categories[i][0].GetClass()) {
            return i;
        }
    }
    return -1;
}

int Utils::SameClass(int c, const std::vector<Instance> &categories) {
    for (int i = 0; i < categories.size(); i++) {
        if (c == categories[i].GetClass()) {
            return i;
        }
    }
    return -1;
}

float Utils::Accuraccy(const std::vector<Instance> &orig, const std::vector<Instance> &predict) {
    float acc = 0;
    for (int i = 0; i < orig.size(); i++) {
        if (orig[i].GetClass() == predict[i].GetClass())
            acc++;
    }
    acc = acc / orig.size();

    return acc;
}

float Utils::Accuraccy(const std::vector<Instance> &orig, const std::vector<int> &predict) {
    float acc = 0;
    for (int i = 0; i < orig.size(); i++) {
        if (orig[i].GetClass() == predict[i])
            acc++;
    }
    acc = acc / orig.size();

    return acc;
}

void Utils::WritePredict(const std::vector<int> &predict, const std::string &resName) {
    std::ofstream myfile;
    myfile.open(resName);
    for (int x: predict) {
        myfile << x << "\n";
    }

    myfile.close();
}

Instance Utils::FindInstance(int id, const std::vector<Instance> &dataset) {
    for (auto instance: dataset) {
        if (instance.GetID() == id) {
            return instance;
        }
    }
    return Instance();
}

/*
 * Case Sensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
bool Utils::StartsWith(const std::string &mainStr, const std::string &toMatch) {
    // std::string::find returns 0 if toMatch is found at starting
    return mainStr.find(toMatch) == 0;
}


/*
 * Case Insensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
bool Utils::StartsWithCaseInsensitive(std::string mainStr, std::string toMatch) {
    // Convert mainStr to lower case
    std::transform(mainStr.begin(), mainStr.end(), mainStr.begin(), ::tolower);
    // Convert toMatch to lower case
    std::transform(toMatch.begin(), toMatch.end(), toMatch.begin(), ::tolower);

    return mainStr.find(toMatch) == 0;
}

bool Utils::CaseInsensitiveStringCompare(std::string &str1, std::string &str2) {
    return ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), [](char &c1, char &c2) {
        return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
    }));
}

std::vector<std::vector<int>> Utils::SplitDataSetIntoClassesIndices(const std::vector<Instance> &dataset) {
    std::vector<int> descriptor(dataset[0].GetDescriptor());
    int labels = descriptor[descriptor.size() - 1];
    std::vector<std::vector<int>> result(labels);
    std::vector<std::list<int>> aux(labels);
    for (int i = 0; i < dataset.size(); i++) {
        aux[dataset[i].GetClass()].push_back(i);
    }
    for (int i = 0; i < labels; i++) {
        result[i] = std::vector<int>(aux[i].begin(), aux[i].end());
    }
    return result;
}

std::vector<std::vector<Instance>> Utils::SplitDataSetIntoClasses(const std::vector<Instance> &dataset) {
    std::vector<int> descriptor(dataset[0].GetDescriptor());
    int labels = descriptor[descriptor.size() - 1];
    int nonEmpty = 0, count = 0;
    std::vector<std::vector<Instance>> result;
    std::vector<std::list<Instance>> aux(labels);
    for (const auto &instance : dataset) {
        aux[instance.GetClass()].push_back(instance);
    }
    for (const auto &category: aux) {
        if (!category.empty()) {
            nonEmpty++;
        }
    }
    result = std::vector<std::vector<Instance>>(nonEmpty);
    for (int i = 0; i < labels; i++) {
        if (!aux[i].empty()) {
            result[count] = std::vector<Instance>(aux[i].begin(), aux[i].end());
            count++;
        }
    }
    return result;
}


