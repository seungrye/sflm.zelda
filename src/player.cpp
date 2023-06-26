#include "player.hpp"
#include <cassert>

Player::Player() {
  auto r = texture.loadFromFile("../src/graphics/player.png");
  assert(r);
  image.setTexture(texture);
  auto rect = image.getTextureRect();
  this->rect = py::Rect(rect);

  // inflate
  hitbox = this->rect.inflate(0, -26);
}
