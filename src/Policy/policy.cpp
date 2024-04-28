#include "VoxelLSTM.h"
#include <iostream>

VoxelLSTM::VoxelLSTM(int input_dim, int hidden_dim, int token_dim)
    : hidden_dim(hidden_dim), token_dim(token_dim),
      input_fc(torch::nn::Linear(input_dim, hidden_dim)),
      token_to_input(torch::nn::Linear(token_dim, hidden_dim)),
      lstm(torch::nn::LSTM(torch::nn::LSTMOptions(hidden_dim, hidden_dim))),
      fc(torch::nn::Linear(hidden_dim, token_dim)) {
    register_module("input_fc", input_fc);
    register_module("token_to_input", token_to_input);
    register_module("lstm", lstm);
    register_module("fc", fc);
}

torch::Tensor VoxelLSTM::forward(torch::Tensor x) {
    x = input_fc(x);
    torch::Tensor empty_x = torch::zeros_like(x);

    // Initialize hidden state and cell state
    torch::Tensor h0 = torch::zeros({1, x.size(1), hidden_dim}).to(x.device());
    torch::Tensor c0 = torch::zeros({1, x.size(1), hidden_dim}).to(x.device());

    // Forward propagate LSTM
    auto out_tuple = lstm(x, std::make_tuple(h0, c0));
    torch::Tensor out = std::get<0>(out_tuple);
    torch::Tensor hn = std::get<1>(out_tuple)[0];
    torch::Tensor cn = std::get<1>(out_tuple)[1];

    // Prepare to collect tokens
    std::vector<torch::Tensor> tokens;
    // Start the autoregression with the output from the initial forward pass
    torch::Tensor current_input = out;  // Taking the last output as the first input to autoregression
    for (int i = 0; i < 6; ++i) {  // Generate 6 tokens
        auto out_tuple = lstm(current_input, std::make_tuple(hn, cn));
        out = std::get<0>(out_tuple);
        hn = std::get<1>(out_tuple)[0];
        cn = std::get<1>(out_tuple)[1];
        torch::Tensor token = fc(out.squeeze(0));
        tokens.push_back(token);
        // Use the output as the next input
        current_input = token_to_input(token);
    }

    // Stack all tokens to form the final output tensor
    torch::Tensor tokens_tensor = torch::stack(tokens, 0);
    tokens_tensor = tokens_tensor.squeeze(2).transpose(0, 1);
    return tokens_tensor;
}

std::vector<float> VoxelLSTM::extract_features(const std::vector<std::map<std::string, int>>& piece_list_item) {
    std::vector<float> features;
    
    for (const auto& voxel : piece_list_item) {
        // Extract relevant features from each voxel
        
        // Example: voxel position, reachability value, piece information, etc.
        features.push_back(voxel.at("pos_x"));
        features.push_back(voxel.at("pos_y"));
        features.push_back(voxel.at("pos_z"));
        features.push_back(voxel.at("reach"));
        features.push_back(voxel.at("piece"));
        
        // Extract neighbor piece and voxel information
        for (int i = 0; i < 6; ++i) {
            features.push_back(voxel.at("neighbor_piece_" + std::to_string(i)));
            features.push_back(voxel.at("neighbor_voxel_" + std::to_string(i)));
        }
    }
    
    return features;
}

std::tuple<torch::Tensor, torch::Tensor> VoxelLSTM::prepare_training_data(const std::vector<std::vector<int>>& piece_list, const std::vector<std::vector<std::vector<int>>>& seed_path_sequence) {
    std::vector<std::vector<float>> X;
    for (const auto& piece_list_item : piece_list) {
        X.push_back(extract_features(piece_list_item));
    }

    std::vector<std::vector<int>> y;
    for (const auto& seed_path : seed_path_sequence) {
        // Extract the output seed path voxels and associated metadata
        const auto& seed_voxels = seed_path[0];
        const auto& kept_voxels = seed_path[1];
        const auto& path_voxels = seed_path[2];

        // Combine the seed voxels and kept voxels into a single label
        std::vector<int> label(seed_voxels);
        label.insert(label.end(), kept_voxels.begin(), kept_voxels.end());
        label.insert(label.end(), path_voxels.begin(), path_voxels.end());

        y.push_back(label);
    }

    return std::make_tuple(torch::tensor(X), torch::tensor(y));
}

void VoxelLSTM::train(const std::vector<std::vector<int>>& piece_list, const std::vector<std::vector<std::vector<int>>>& seed_path_sequence, int num_epochs, int batch_size) {
    auto [X, y] = prepare_training_data(piece_list, seed_path_sequence);

    // Define the loss function and optimizer
    torch::nn::CrossEntropyLoss criterion;
    torch::optim::Adam optimizer(parameters());

    // Training loop
    for (int epoch = 0; epoch < num_epochs; ++epoch) {
        for (int i = 0; i < X.size(0); i += batch_size) {
            auto batch_X = X.slice(0, i, i + batch_size).unsqueeze(1);
            auto batch_y = y.slice(0, i, i + batch_size);

            // Forward pass
            auto outputs = forward(batch_X);
            auto loss = criterion(outputs.reshape({-1, token_dim}), batch_y.reshape({-1}));

            // Backward and optimize
            optimizer.zero_grad();
            loss.backward();
            optimizer.step();
        }

        // Calculate accuracy
        with(torch::no_grad(), [&] {
            auto outputs = forward(X.unsqueeze(1));
            auto predicted = outputs.argmax(-1);
            auto accuracy = (predicted == y).to(torch::kFloat32).mean();
            std::cout << "Epoch [" << epoch + 1 << "/" << num_epochs << "], Loss: " << loss.item<float>() << ", Accuracy: " << accuracy.item<float>() << std::endl;
        });
    }
}