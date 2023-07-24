#ifndef __LEVEL_HPP__
#define __LEVEL_HPP__

#include "game_window.hpp"
#include "player.hpp"
#include "pygame_adapter.hpp"
#include "support.hpp"
#include "enemy.hpp"
#include "ui.hpp"
#include "magic.hpp"
#include "particles.hpp"
#include "upgrade.hpp"
#include "actions.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <list>

class YSortCameraGroup
{
public:
  YSortCameraGroup();
  void custom_draw(std::shared_ptr<Player> player);
  void update();
  void enemy_update(std::shared_ptr<Player> player);
  void push_back(std::shared_ptr<SpriteTexture> __x) { this->sprites.push_back(__x); }
  void remove(const std::shared_ptr<SpriteTexture> &__x) { this->sprites.remove(__x); }

private:
  unsigned half_width;
  unsigned half_height;
  sf::Vector2f offset;

  SpriteTexture floor;
  std::list<std::shared_ptr<SpriteTexture>> sprites;
};

class CreateAttack;

class Level
{
public:
  Level();

  void create_map();
  void destroy_attack();
  void run();
  void toggle_menu() { this->game_paused = !this->game_paused; }

private:
  void player_attack_logic();

  template <typename T>
  auto random_choice(const T &list) { return list[random() % list.size()]; }

private:
  friend class CreateAttack;
  YSortCameraGroup visible_sprites;
  std::list<std::shared_ptr<SpriteTexture>> obstacle_sprites;
  std::list<std::shared_ptr<SpriteTexture>> attackable_sprites;
  std::vector<std::shared_ptr<SpriteTexture>> attack_sprites;
  std::shared_ptr<Player> player;
  std::shared_ptr<SpriteTexture> current_attack;
  UI ui;
  std::shared_ptr<Upgrade> upgrade;
  std::shared_ptr<AnimationPlayer> animation_player;
  // std::shared_ptr<MagicPlayer> magic_player;

  bool game_paused;
};

#endif
