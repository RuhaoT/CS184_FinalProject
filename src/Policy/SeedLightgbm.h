#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <nlohmann/json.hpp>
#include <lightgbm/lightgbm_api.h>

using json = nlohmann::json;

json readJsonFile(const std::string& file);
std::vector<int> flattenWithPadding(const std::vector<std::vector<int>>& data, int maxLength, int padValue = -1);
std::pair<std::vector<std::vector<double>>, std::vector<int>> preprocessData(const json& data, int maxPathVoxelsLength);

template <typename T>
void shuffleData(std::vector<T>& X, std::vector<int>& y);

template <typename T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<int>, std::vector<int>> splitData(std::vector<T>& X, std::vector<int>& y, double testSize);

double calculateAccuracy(const std::vector<int>& yTrue, const std::vector<int>& yPred);

#endif // MAIN_H