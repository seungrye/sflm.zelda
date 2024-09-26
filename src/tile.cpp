#include "tile.hpp"
#include "settings.hpp"

Tile::Tile(const sf::Vector2f &pos, std::string sprite_type)
    : SpriteTexture(TILESIZE, TILESIZE)
{
  this->sprite_type_ = sprite_type;
  this->init(pos);
}

Tile::Tile(const sf::Vector2f &pos, std::string sprite_type,
           const SpriteTexture &sprite)
    : SpriteTexture(sprite)
{
  this->sprite_type_ = sprite_type;
  this->init(pos);
}

void Tile::init(const sf::Vector2f &pos)
{
  if (!sprite_type_.compare("object"))
  {
    this->get_rect({"topleft", sf::Vector2f(pos.x, pos.y - TILESIZE)});
  }
  else
  {
    this->get_rect({"topleft", pos});
  }
  const auto y_offset = HITBOX_OFFSET[this->sprite_type_];
  this->hitbox_ = this->rect_.inflate(0, y_offset);
}
