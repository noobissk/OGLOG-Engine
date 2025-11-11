#pragma once
#include <filesystem>
#include <iostream>

using path = std::filesystem::path;


path asset_directory = std::filesystem::current_path() / "assets";

void run_test_path_log() {
    std::cout << asset_directory << std::endl;
};