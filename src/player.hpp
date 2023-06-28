#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class Player : public SpriteTexture {
public:
  Player() = default;
  Player(const sf::Vector2u &pos, std::vector<std::shared_ptr<SpriteTexture>> obstacle_sprites);

private:
  py::Rect hitbox;
};

#endif
