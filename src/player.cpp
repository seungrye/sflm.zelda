#include "player.hpp"
#include "support.hpp"
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <memory>
#include <vector>

Player::Player(const sf::Vector2u &pos,
               std::vector<std::shared_ptr<SpriteTexture>> obstacle_sprites) {
  auto r = this->loadFromFile("./src/graphics/player.png");
  assert(r);
  this->get_rect({"topleft", pos});

  // inflate
  this->hitbox = this->rect_.inflate(0, -26);
}
