#ifndef __SUPPORT_HPP__
#define __SUPPORT_HPP__

#include "pygame_adapter.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

class SpriteTexture {
public:
  SpriteTexture() : texture_(std::make_shared<sf::Texture>()) {}
  SpriteTexture(unsigned width, unsigned height) : texture_(std::make_shared<sf::Texture>()) {
    auto r = this->texture_->create(width, height);
    assert(r);
    this->sprite_.setTexture(*this->texture_);
    auto rect = this->sprite_.getTextureRect();
    this->rect_ = py::Rect(rect);
  }
  SpriteTexture(const std::string &path) : texture_(std::make_shared<sf::Texture>()) {
    auto r = this->loadFromFile(path);
    assert(r);
  }
  bool loadFromFile(const std::string &filename,
                    const sf::IntRect &area = sf::IntRect()) {
    auto r = this->texture_->loadFromFile(filename);
    if (r) {
      this->sprite_.setTexture(*this->texture_);
      auto rect = this->sprite_.getTextureRect();
      this->rect_ = py::Rect(rect);
    }
    return r;
  }

  const sf::IntRect &getTextureRect() { return this->sprite_.getTextureRect(); }

  void move(const sf::Vector2f &offset) { this->sprite_.setOrigin(-offset); }

  const sf::Sprite &surf() { return this->sprite_; }

  const py::Rect &get_rect(const std::pair<std::string, sf::Vector2u> &pos) {
    if (!pos.first.compare("topleft")) {
      this->rect_ = py::Rect(pos.second.x, pos.second.y, this->rect_.width,
                             this->rect_.height);
    }
    return this->rect_;
  }

  const py::Rect &rect() { return this->rect_; }

protected:
  std::shared_ptr<sf::Texture> texture_; //! 필수
  sf::Sprite sprite_;
  py::Rect rect_;
};

std::vector<std::vector<std::string>> import_csv_layout(const std::string &filename);
std::vector<std::shared_ptr<SpriteTexture>> import_folder(const std::string &folder);

#endif /* __SUPPORT_HPP__ */
