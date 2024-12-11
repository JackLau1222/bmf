#include <builder.hpp>
#include "nlohmann/json.hpp"
#include <unistd.h>
// #include <bmf/sdk/bmf.h>

int main() {
    std::string input_video_path = "../../files/big_bunny_10s_30fps.mp4";
    std::string input_video_path2 = "../../files/big_bunny_10s_30fps.mp4";
    std::string output_path = "./output.mp4";

    auto main_graph = bmf::builder::Graph(bmf::builder::NormalMode);
    nlohmann::json video1_param = {
        {"input_path", input_video_path},
        {"alias", "decoder0"}
    };
    auto video1 = main_graph.Decode(JsonParam(video1_param));

    nlohmann::json passthu_param = {
        {"alias", "pass_through"}
    };
    auto passthu = main_graph.Module({video1["video"], video1["audio"]},
                                     "pass_through", bmf::builder::CPP,
                                     bmf_sdk::JsonParam(passthu_param), ""
                                     "", "");
    passthu.Start();
    sleep(0.1);

    auto update_decoder = bmf::builder::Graph(bmf::builder::NormalMode);
    nlohmann::json video2_param = {
        {"input_path", input_video_path2},
        {"alias", "decoder1"}
    };
    auto video2 = update_decoder.Decode(JsonParam(video2_param));

    nlohmann::json outputs = {
        {"alias", "pass_through"},
        {"streams", 2}
    };
    update_decoder.DynamicAdd(std::make_shared<bmf::builder::Stream>(video2), nullptr, std::make_shared<nlohmann::json>(outputs));
    sleep(0.03);

    auto update_encoder = bmf::builder::Graph(bmf::builder::NormalMode);
    nlohmann::json encode_param = {
        {"output_path", output_path},
        {"alias", "encoder1"}
    };
    auto encode = update_encoder.Encode(JsonParam(encode_param));
    nlohmann::json inputs = {
        {"alias", "pass_through"},
        {"streams", 2}
    };
    update_encoder.DynamicAdd(std::make_shared<bmf::builder::Stream>(encode), std::make_shared<nlohmann::json>(inputs), nullptr);
    main_graph.Update(std::make_shared<bmf::builder::Graph>(update_encoder));
    sleep(0.05);

    auto remove_graph = bmf::builder::Graph(bmf::builder::NormalMode);
    // remove_graph.DynamicRemove()
    // main_graph.Update(std::make_shared<bmf::builder::Graph>(remove_graph));
    main_graph.Close();

    return 0;
}