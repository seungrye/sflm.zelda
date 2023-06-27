#ifndef __TILE_HPP__
#define __TILE_HPP__

#include "pygame_adapter.hpp"
#include "settings.hpp"
#include "support.hpp"
#include <SFML/System/Vector2.hpp>

class Tile: public SpriteTexture {
public:
  Tile(const sf::Vector2u &pos,
       std::vector<std::vector<SpriteTexture>>& sprite_groups,
       std::string sprite_type)
      : 
      // sprite_(SpriteTexture(TILESIZE, TILESIZE)), 
      SpriteTexture(TILESIZE, TILESIZE),
      sprite_type_(sprite_type) {
    this->init(pos);
    for (auto &sprite_group : sprite_groups) {
      // sprite_group.push_back(this->sprite_);
      sprite_group.push_back(*this);
    }
  }
  Tile(const sf::Vector2u &pos,
       std::vector<std::vector<SpriteTexture>>& sprite_groups,
       std::string sprite_type, SpriteTexture sprite)
      : 
      // sprite_(sprite), 
      sprite_type_(sprite_type) {
    this->init(pos);
    for (auto &sprite_group : sprite_groups) {
      // sprite_group.push_back(this->sprite_);
      sprite_group.push_back(*this);
    }
  }

private:
  void init(const sf::Vector2u &pos) {
    if (!sprite_type_.compare("object")) {
      // this->rect_ = this->sprite_.get_rect(
      //     {"topleft", sf::Vector2u(pos.x, pos.y - TILESIZE)});
      this->get_rect(
          {"topleft", sf::Vector2u(pos.x, pos.y - TILESIZE)});
    } else {
      // this->rect_ = this->sprite_.get_rect({"topleft", pos});
      this->get_rect({"topleft", pos});
    }
  }

private:
  std::string sprite_type_;
  // SpriteTexture sprite_;
  py::Rect hitbox_;
  // py::Rect rect_;
};

#endif /* __TILE_HPP__ */
