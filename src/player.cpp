#include "player.hpp"
#include "support.hpp"
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <vector>

Player::Player(const sf::Vector2u &pos,
               std::vector<std::vector<SpriteTexture>> sprite_groups,
               std::vector<SpriteTexture> obstacle_sprites) {
  auto r = this->sprite_.loadFromFile("../src/graphics/player.png");
  assert(r);
  this->rect = this->sprite_.get_rect({"topleft", pos});
  for (auto& sprite_group : sprite_groups) {
    sprite_group.push_back(this->sprite_);
  }

  // inflate
  hitbox = this->rect.inflate(0, -26);
}
