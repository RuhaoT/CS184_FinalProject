#include "SeedLightgbm.h"

json readJsonFile(const std::string& file) {
    std::ifstream r(file);
    std::stringstream buffer;
    buffer << r.rdbuf();
    std::string response = buffer.str();
    response.erase(std::remove(response.begin(), response.end(), '\n'), response.end());
    std::replace(response.begin(), response.end(), '}', '},');
    response = "[" + response + "]";
    return json::parse(response);
}

std::vector<int> flattenWithPadding(const std::vector<std::vector<int>>& data, int maxLength, int padValue = -1) {
    std::vector<int> flatList;
    for (const auto& sublist : data) {
        flatList.insert(flatList.end(), sublist.begin(), sublist.end());
        if (flatList.size() >= maxLength * 3) {
            break;
        }
    }
    flatList.resize(maxLength * 3, padValue);
    return flatList;
}

std::pair<std::vector<std::vector<double>>, std::vector<int>> preprocessData(const json& data, int maxPathVoxelsLength) {
    std::vector<std::vector<double>> features;
    std::vector<int> labels;

    for (const auto& item : data) {
        std::vector<double> featureVector;
        labels.push_back(item["isFinalResultValid"]);

        for (const auto& sequence : item["seedPathSequence"]) {
            featureVector.push_back(sequence["moveStep"]);
            featureVector.push_back(sequence["pieceAxisID"]);
            featureVector.push_back(sequence["seedPossib"]);

            std::vector<std::vector<int>> pathVoxels = sequence["pathVoxels"];
            std::vector<int> flattenedPathVoxels = flattenWithPadding(pathVoxels, maxPathVoxelsLength);
            featureVector.insert(featureVector.end(), flattenedPathVoxels.begin(), flattenedPathVoxels.end());

            std::vector<std::vector<int>> seedVoxels = sequence["seedVoxels"];
            std::vector<int> flattenedSeedVoxels = flattenWithPadding(seedVoxels, 1);
            featureVector.insert(featureVector.end(), flattenedSeedVoxels.begin(), flattenedSeedVoxels.end());

            std::vector<std::vector<int>> keptVoxels = sequence["keptVoxels"];
            std::vector<int> flattenedKeptVoxels = flattenWithPadding(keptVoxels, 1);
            featureVector.insert(featureVector.end(), flattenedKeptVoxels.begin(), flattenedKeptVoxels.end());

            featureVector.push_back(sequence["contaVoxels"].size());
            featureVector.push_back(sequence["moveAxisIDs"].size());
        }

        features.push_back(featureVector);
    }

    return std::make_pair(features, labels);
}

template <typename T>
void shuffleData(std::vector<T>& X, std::vector<int>& y) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(X.begin(), X.end(), rng);
    std::shuffle(y.begin(), y.end(), rng);
}

template <typename T>
std::tuple<std::vector<T>, std::vector<T>, std::vector<int>, std::vector<int>> splitData(std::vector<T>& X, std::vector<int>& y, double testSize) {
    int splitIndex = static_cast<int>(X.size() * (1.0 - testSize));
    std::vector<T> XTrain(X.begin(), X.begin() + splitIndex);
    std::vector<T> XTest(X.begin() + splitIndex, X.end());
    std::vector<int> yTrain(y.begin(), y.begin() + splitIndex);
    std::vector<int> yTest(y.begin() + splitIndex, y.end());
    return std::make_tuple(XTrain, XTest, yTrain, yTest);
}

double calculateAccuracy(const std::vector<int>& yTrue, const std::vector<int>& yPred) {
    int correctPredictions = 0;
    for (size_t i = 0; i < yTrue.size(); ++i) {
        if (yTrue[i] == yPred[i]) {
            ++correctPredictions;
        }
    }
    return static_cast<double>(correctPredictions) / yTrue.size();
}
