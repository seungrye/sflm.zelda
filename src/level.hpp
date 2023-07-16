#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include "game_window.hpp"
#include "player.hpp"
#include "pygame_adapter.hpp"
#include "support.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include "enemy.hpp"

class YSortCameraGroup
{
public:
  YSortCameraGroup();
  void custom_draw(std::shared_ptr<Player> player);
  void update();
  void enemy_update(std::shared_ptr<Player> player) {
    for(const auto& sprite: this->sprites) {
      if (sprite->is("enemy")) {
        auto enemy = std::dynamic_pointer_cast<Enemy>(sprite);
        enemy->enemy_update(player);
      }
    }
  }
  void push_back(std::shared_ptr<SpriteTexture> __x) { this->sprites.push_back(__x); }

private:
  unsigned half_width;
  unsigned half_height;
  sf::Vector2f offset;

  SpriteTexture floor;
  std::vector<std::shared_ptr<SpriteTexture>> sprites;
};

class Level
{
public:
  Level();
  void create_map();
  void create_attack();
  void destroy_attack();
  void run();

private:
  auto random_choice(const std::vector<std::shared_ptr<SpriteTexture>> &list)
  {
    return list[random() % list.size()];
  }

private:
  YSortCameraGroup visible_sprites;
  std::vector<std::shared_ptr<SpriteTexture>> obstacle_sprites;
  std::vector<std::shared_ptr<SpriteTexture>> attackable_sprites;
  std::vector<std::shared_ptr<SpriteTexture>> attack_sprites;
  std::shared_ptr<Player> player;

  bool game_paused;
};

#endif
