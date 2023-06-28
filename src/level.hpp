#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include "game_window.hpp"
#include "player.hpp"
#include "pygame_adapter.hpp"
#include "support.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

class YSortCameraGroup {
public:
  YSortCameraGroup();
  void custom_draw(Player &player);
  void update();
  void push_back(std::shared_ptr<SpriteTexture> __x) { this->sprites.push_back(__x); }

private:
  unsigned half_width;
  unsigned half_height;
  sf::Vector2i offset;

  SpriteTexture floor;
  std::vector<std::shared_ptr<SpriteTexture>> sprites;
};

class Level {
public:
  Level();
  void create_map();
  void create_attack();
  void destroy_attack();
  void run();

private:
  YSortCameraGroup visible_sprites;
  std::vector<std::shared_ptr<SpriteTexture>> obstacle_sprites;
  Player player;
};

#endif
