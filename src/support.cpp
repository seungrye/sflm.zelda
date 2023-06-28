#include "support.hpp"
#include "level.hpp"
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> import_csv_layout(const std::string &filename) {
  auto terrain_map = std::vector<std::vector<std::string>>();

  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line)) {
    std::vector<std::string> row;
    std::stringstream line_stream(line);
    std::string cell;
    while (std::getline(line_stream, cell, ',')) {
      row.push_back(cell);
    }
    terrain_map.push_back(row);
  }

  return std::move(terrain_map);
}

std::vector<std::shared_ptr<SpriteTexture>> import_folder(const std::string &folder) {
  auto surface_list = std::vector<std::shared_ptr<SpriteTexture>>{};

  for (const auto &entry :
       std::experimental::filesystem::directory_iterator(folder)) {
    const auto &path = entry.path();

    if (std::experimental::filesystem::is_regular_file(entry)) {
      surface_list.push_back(std::make_shared<SpriteTexture>(path));
    }
  }

  return surface_list;
}