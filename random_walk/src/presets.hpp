#pragma once

#include <nlohmann/json.hpp>

#include <vector>
#include <fstream>
#include <string>

using json = nlohmann::json;

struct Preset {
    std::string displayName;
    unsigned int seed;
    unsigned int n;
    unsigned int step;
};

std::vector<Preset> getPresets() {
    std::vector<Preset> presets;

    std::ifstream f("presets.json");
    json data = json::parse(f);

    for (const auto& item : data) {
        presets.emplace_back(item["displayName"].get<std::string>(),
                            item["seed"].get<unsigned int>(),
                            item["n"].get<unsigned int>(),
                            item["step"].get<unsigned int>());
    }

    return presets;
}