#pragma once

#include "Instance.h"
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <boost/algorithm/string.hpp>
#include <regex>

class Utils {
public:
    Utils();

    static std::vector<Instance> ReadARFF(const std::string &training);

    static std::vector<Instance> ReadCSV(const std::string &training);

    static bool CompareInstances(const std::vector<Instance> &inst1, const std::vector<Instance> &inst2);

    static int Max(const std::vector<int> &c);

    static int SameClass(int c, const std::vector<std::vector<Instance>> &categories);

    static int SameClass(int c, const std::vector<Instance> &categories);

    static float Accuraccy(const std::vector<Instance> &orig, const std::vector<Instance> &predict);

    static float Accuraccy(const std::vector<Instance> &orig, const std::vector<int> &predict);

    static void WritePredict(const std::vector<int> &predict, const std::string &resName);

    static bool StartsWith(const std::string &mainStr, const std::string &toMatch);

    static bool StartsWithCaseInsensitive(std::string mainStr, std::string toMatch);

    static bool CaseInsensitiveStringCompare(std::string &str1, std::string &str2);

    static Instance FindInstance(int id, const std::vector<Instance> &dataset);

    static std::vector<std::vector<int>> SplitDataSetIntoClassesIndices(const std::vector<Instance> &dataset);

    static std::vector<std::vector<Instance>> SplitDataSetIntoClasses(const std::vector<Instance> &dataset);

private:


};