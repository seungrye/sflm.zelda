#include "support.hpp"
#include "level.hpp"
#include "pygame_adapter.hpp"
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

SpriteTexture::SpriteTexture() : texture_(std::make_shared<sf::Texture>()) {}

SpriteTexture::SpriteTexture(unsigned width, unsigned height)
    : texture_(std::make_shared<sf::Texture>()) {
  auto r = this->texture_->create(width, height);
  assert(r);
  this->sprite_.setTexture(*this->texture_);
  auto rect = this->sprite_.getTextureRect();
  this->rect_ = py::Rect(rect);
}

SpriteTexture::SpriteTexture(const std::string &path)
    : texture_(std::make_shared<sf::Texture>()) {
  auto r = this->loadFromFile(path);
  assert(r);
}

bool SpriteTexture::loadFromFile(const std::string &filename,
                                 const sf::IntRect &area) {
  auto r = this->texture_->loadFromFile(filename);
  if (r) {
    this->sprite_.setTexture(*this->texture_);
    auto rect = this->sprite_.getTextureRect();
    this->rect_ = py::Rect(rect);
  }
  return r;
}

const sf::IntRect &SpriteTexture::getTextureRect() {
  return this->sprite_.getTextureRect();
}

void SpriteTexture::move(const sf::Vector2f &offset) {
  this->sprite_.setOrigin(-offset);
}

const sf::Sprite &SpriteTexture::surf() { return this->sprite_; }

const py::Rect &
SpriteTexture::get_rect(const std::pair<std::string, sf::Vector2u> &pos) {
  if (!pos.first.compare("topleft")) {
    this->rect_ = py::Rect(pos.second.x, pos.second.y, this->rect_.width,
                           this->rect_.height);
  } else if (!pos.first.compare("center")) {
    auto diff = this->rect_.center() - pos.second;
    this->rect_ = py::Rect(this->rect_.left + diff.x, this->rect_.top + diff.y,
                           this->rect_.width, this->rect_.height);
  }

  return this->rect_;
}

const py::Rect &SpriteTexture::rect() { return this->rect_; }

std::vector<std::vector<std::string>>
import_csv_layout(const std::string &filename) {
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

std::vector<std::shared_ptr<SpriteTexture>>
import_folder(const std::string &folder) {
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