#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include <SFML/Graphics.hpp>

class Player {
public:
  Player();

public:
  py::Rect rect;

private:
  sf::Texture texture;
  sf::Sprite image;
  py::Rect hitbox;
};

#endif
