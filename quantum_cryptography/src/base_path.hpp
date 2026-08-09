#pragma once

#include <string>
#include <filesystem>

const auto basePath = std::filesystem::current_path().parent_path();
