#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include <SFML/Graphics.hpp>

class Player {
public:
  Player() = default;
  Player(const sf::Vector2u &pos,
         std::vector<std::vector<SpriteTexture>> sprite_group,
         std::vector<SpriteTexture> obstacle_sprites);

public:
  py::Rect rect;

private:
  // sf::Texture texture;
  // sf::Sprite image;
  SpriteTexture sprite_;
  py::Rect hitbox;
};

#endif
