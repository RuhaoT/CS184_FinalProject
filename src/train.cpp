#include "RandomForest/DecisionTree.h"
#include "RandomForest/Data.h"
#include "RandomForest/RandomForest.h"

#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <random>
#include <algorithm>

using json = nlohmann::json;

int main() {
    std::ifstream file("data/Modified_Spider_12x13x6.json");
    json jsonData;
    file >> jsonData;

    // Convert JSON data to the format used in the example data
    std::vector<std::string> trainData;
    for (const auto& item : jsonData) {
        std::string line = item["isFinalResultValid"] ? "1" : "0";
        if (item.contains("seedPathSequence") && item["seedPathSequence"].is_array()) {
            for (const auto& seq : item["seedPathSequence"]) {
                if (seq.is_object()) {
                    if (seq.contains("moveStep") && seq["moveStep"].is_number_integer())
                        line += " 5:" + std::to_string(seq["moveStep"].get<int>());
                    if (seq.contains("pieceAxisID") && seq["pieceAxisID"].is_number_integer())
                        line += " 6:" + std::to_string(seq["pieceAxisID"].get<int>());
                    if (seq.contains("seedPossib") && seq["seedPossib"].is_number_float())
                        line += " 7:" + std::to_string(seq["seedPossib"].get<double>());

                    int pathVoxelsIndex = 8;
                    if (seq.contains("pathVoxels") && seq["pathVoxels"].is_array()) {
                        for (const auto& voxel : seq["pathVoxels"]) {
                            if (voxel.is_array() && voxel.size() == 3) {
                                line += " " + std::to_string(pathVoxelsIndex) + ":" + std::to_string(voxel[0].get<int>());
                                line += " " + std::to_string(pathVoxelsIndex + 1) + ":" + std::to_string(voxel[1].get<int>());
                                line += " " + std::to_string(pathVoxelsIndex + 2) + ":" + std::to_string(voxel[2].get<int>());
                                pathVoxelsIndex += 3;
                            }
                        }
                    }

                    int seedVoxelsIndex = 50;
                    if (seq.contains("seedVoxels") && seq["seedVoxels"].is_array()) {
                        for (const auto& voxel : seq["seedVoxels"]) {
                            if (voxel.is_array() && voxel.size() == 3) {
                                line += " " + std::to_string(seedVoxelsIndex) + ":" + std::to_string(voxel[0].get<int>());
                                line += " " + std::to_string(seedVoxelsIndex + 1) + ":" + std::to_string(voxel[1].get<int>());
                                line += " " + std::to_string(seedVoxelsIndex + 2) + ":" + std::to_string(voxel[2].get<int>());
                                seedVoxelsIndex += 3;
                            }
                        }
                    }

                    int keptVoxelsIndex = 60;
                    if (seq.contains("keptVoxels") && seq["keptVoxels"].is_array()) {
                        for (const auto& voxel : seq["keptVoxels"]) {
                            if (voxel.is_array() && voxel.size() == 3) {
                                line += " " + std::to_string(keptVoxelsIndex) + ":" + std::to_string(voxel[0].get<int>());
                                line += " " + std::to_string(keptVoxelsIndex + 1) + ":" + std::to_string(voxel[1].get<int>());
                                line += " " + std::to_string(keptVoxelsIndex + 2) + ":" + std::to_string(voxel[2].get<int>());
                                keptVoxelsIndex += 3;
                            }
                        }
                    }

//                     if (seq.contains("contaVoxels") && seq["contaVoxels"].is_array())
//                         line += " 144:" + std::to_string(seq["contaVoxels"].size());
//                     if (seq.contains("moveAxisIDs") && seq["moveAxisIDs"].is_array())
//                         line += " 145:" + std::to_string(seq["moveAxisIDs"].size());
                }
            }
        }
        trainData.push_back(line);
    }

    // Shuffle the trainData randomly
//     std::random_device rd;
//     std::mt19937 rng(rd());
//     std::shuffle(trainData.begin(), trainData.end(), rng);

    // Split the trainData into training set and test set
    std::size_t testSize = trainData.size() * 0.1; // 10% for testing
    std::vector<std::string> testData(trainData.begin(), trainData.begin() + testSize);
//     trainData.erase(trainData.begin(), trainData.begin() + testSize);

    // Print the converted data
    std::cout << "Training Data:" << std::endl;
    for (const auto& line : trainData) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Testing Data:" << std::endl;
    for (const auto& line : testData) {
        std::cout << line << std::endl;
    }
    std::cout << std::endl;

    // Load the training data into the Data object
    Data trainingData(true, trainData.size(), 100);
    trainingData.read(trainData);
    
    std::cout << "Training Data Size: " << trainingData.getSampleSize() << std::endl;

    RandomForest randomForest(10, "gini", "log2", -1, 20, 1, 1000000, 8);
    
    
    randomForest.fit(trainingData);
    
    std::cout << "Training Finishes " << std::endl;

    

    // Load the test data into the Data object
    Data testingData(true, testData.size(), 100);
    testingData.read(testData);

    auto results = randomForest.predictProba(testingData);
    writeDataToCSV(results, testingData, "testResults.csv", true);

    return 0;
}