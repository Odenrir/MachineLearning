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
    std::string missing("?");
    int numericValues = 0, nominalValues = 0, id = 0;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (!dataReading && StartsWithCaseInsensitive(line, attribute)) {
                boost::split(splitString, line, boost::is_any_of(" "));

                if (CaseInsensitiveStringCompare(splitString[2], numeric)) {
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
            } else if (dataReading && !StartsWithCaseInsensitive(line, description)) {
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
                inst.SetID(id);
                inst.SetClass(vectorMapNominal[k][splitString[descriptorVector.size() - 1]]);
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

void Utils::PrintInstance(Instance &inst) {
    std::vector<float> feat = inst.GetNumericFeatures();
    for (int i = 0; i < feat.size(); i++) {
        std::cout << feat[i] << ", ";
    }
    std::cout << "\n";
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
    int index = -1, maxV = INT32_MIN, i;
    for (i = 0; i < c.size(); i++) {
        if (c[i] > maxV) {
            maxV = c[i];
            index = i;
        }
    }
    return index;
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
    for (int i = 0; i < predict.size(); i++) {
        myfile << predict[i] << "\n";
    }

    myfile.close();
}

/*
 * Case Sensitive Implementation of startsWith()
 * It checks if the string 'mainStr' starts with given string 'toMatch'
 */
bool Utils::StartsWith(std::string mainStr, std::string toMatch) {
    // std::string::find returns 0 if toMatch is found at starting

    if (mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
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

    if (mainStr.find(toMatch) == 0)
        return true;
    else
        return false;
}

bool Utils::CaseInsensitiveStringCompare(std::string &str1, std::string &str2) {
    return ((str1.size() == str2.size()) && std::equal(str1.begin(), str1.end(), str2.begin(), [](char &c1, char &c2) {
        return (c1 == c2 || std::toupper(c1) == std::toupper(c2));
    }));
}