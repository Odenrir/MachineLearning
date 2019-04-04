#pragma once

#include "Instance.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <boost/algorithm/string.hpp>

class Utils {
public:
    Utils();

    static std::vector<Instance> ReadARFF(const std::string &training);

    static Instance StringToInstanceRICATIM(std::string &line);

    static std::vector<Instance> ReadCSVRICATIM(const std::string &training);

    static void ReadLabelsRICATIM(const std::string &labels, std::vector<Instance> &instances);

    static void PrintInstance(Instance &inst);

    static bool CompareInstances(const std::vector<Instance> &inst1, const std::vector<Instance> &inst2);

    static int Max(const std::vector<int> &c);

    static float Accuraccy(const std::vector<Instance> &orig, const std::vector<Instance> &predict);

    static float Accuraccy(const std::vector<Instance> &orig, const std::vector<int> &predict);

    static void WritePredict(const std::vector<int> &predict, const std::string &resName);

    static bool StartsWith(std::string mainStr, std::string toMatch);

    static bool StartsWithCaseInsensitive(std::string mainStr, std::string toMatch);

    static bool CaseInsensitiveStringCompare(std::string &str1, std::string &str2);

private:


};