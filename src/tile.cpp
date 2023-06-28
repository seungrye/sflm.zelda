#include "tile.hpp"

Tile::Tile(const sf::Vector2u &pos, std::string sprite_type)
    : SpriteTexture(TILESIZE, TILESIZE), sprite_type_(sprite_type) {
  this->init(pos);
}

Tile::Tile(const sf::Vector2u &pos, std::string sprite_type,
           const SpriteTexture &sprite)
    : SpriteTexture(sprite), sprite_type_(sprite_type) {
  this->init(pos);
}

void Tile::init(const sf::Vector2u &pos) {
  if (!sprite_type_.compare("object")) {
    this->get_rect({"topleft", sf::Vector2u(pos.x, pos.y - TILESIZE)});
  } else {
    this->get_rect({"topleft", pos});
  }
}