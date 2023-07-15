#ifndef __TILE_HPP__
#define __TILE_HPP__

#include "pygame_adapter.hpp"
#include "settings.hpp"
#include "support.hpp"
#include <SFML/System/Vector2.hpp>

class Tile : public SpriteTexture {
public:
  Tile(const sf::Vector2u &pos, std::string sprite_type);
  Tile(const sf::Vector2u &pos, std::string sprite_type, const SpriteTexture& sprite);

private:
  void init(const sf::Vector2u &pos);

private:
  std::string sprite_type_;
};

#endif /* __TILE_HPP__ */
