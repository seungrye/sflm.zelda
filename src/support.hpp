#ifndef __SUPPORT_HPP__
#define __SUPPORT_HPP__

#include "pygame_adapter.hpp"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class SpriteTexture {
public:
  bool loadFromFile(const std::string &filename,
                    const sf::IntRect &area = sf::IntRect()) {
    auto r = this->texture_.loadFromFile(filename);
    if (r) {
      this->sprite_.setTexture(this->texture_);
      auto rect = this->sprite_.getTextureRect();
      this->rect_ = py::Rect(rect);
    }
    return r;
  }

  const sf::IntRect &getTextureRect() { return this->sprite_.getTextureRect(); }

  void move(const sf::Vector2f &offset) { this->sprite_.move(offset); }

  const sf::Sprite &surf() { return this->sprite_; }

  const py::Rect &rect() { return this->rect_; }

private:
  sf::Texture texture_;
  sf::Sprite sprite_;
  py::Rect rect_;
};

std::vector<std::vector<std::string>> import_csv_layout(const std::string &filename);
std::vector<SpriteTexture> import_folder(const std::string &folder);

#endif /* __SUPPORT_HPP__ */
