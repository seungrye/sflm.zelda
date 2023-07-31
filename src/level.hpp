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
#include "sprite_manager.hpp"
#include "ysort_camera_group.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <list>

class Level
{
public:
  Level();

  void create_map();
  void run();
  void toggle_menu() { this->game_paused = !this->game_paused; }

private:
  void player_attack_logic();

  template <typename T>
  auto random_choice(const T &list) { return list[random() % list.size()]; }

private:
  void create_attack();
  void destroy_attack();
  void create_magic(const std::string &style, int strength, int cost);
  void destroy_magic();
  void damage_player(int amount, const std::string &attack_type);
  void trigger_death_particles(const sf::Vector2f &pos, const std::string &particle_type);
  void add_exp(int amount);

private:
  YSortCameraGroup visible_sprites;
  std::list<std::shared_ptr<SpriteTexture>> obstacle_sprites;
  std::list<std::shared_ptr<SpriteTexture>> attackable_sprites;
  std::list<std::shared_ptr<SpriteTexture>> attack_sprites;
  std::shared_ptr<Player> player;
  std::shared_ptr<SpriteTexture> current_attack;
  UI ui;
  std::shared_ptr<Upgrade> upgrade;
  std::shared_ptr<AnimationPlayer> animation_player;
  std::shared_ptr<MagicPlayer> magic_player;

  SpriteManager sprite_manager;

  bool game_paused;
};

#endif
