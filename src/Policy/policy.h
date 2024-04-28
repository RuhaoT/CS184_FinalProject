#ifndef VOXEL_LSTM_H
#define VOXEL_LSTM_H

#include <torch/torch.h>
#include <vector>

class VoxelLSTM : public torch::nn::Module {
public:
    VoxelLSTM(int input_dim, int hidden_dim, int token_dim);
    torch::Tensor forward(torch::Tensor x);
    void train(const std::vector<std::vector<int>>& piece_list, const std::vector<std::vector<std::vector<int>>>& seed_path_sequence, int num_epochs, int batch_size);

private:
    int hidden_dim;
    int token_dim;
    torch::nn::Linear input_fc;
    torch::nn::Linear token_to_input;
    torch::nn::LSTM lstm;
    torch::nn::Linear fc;

    std::vector<float> extract_features(const std::vector<std::map<std::string, int>>& piece_list_item);
    std::tuple<torch::Tensor, torch::Tensor> prepare_training_data(const std::vector<std::vector<int>>& piece_list, const std::vector<std::vector<std::vector<int>>>& seed_path_sequence);
};

#endif // VOXEL_LSTM_H