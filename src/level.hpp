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
#include "sprite_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include <vector>
#include <iostream>
#include <list>

class Level : public SpriteManager
{
public:
  Level();

  void create_map();
  void run();
  void toggle_menu() { this->game_paused = !this->game_paused; }

  void kill(const SpriteTexture *sprite_texture) override
  {
    auto found = this->visible_sprites.find_if(
        [sprite_texture](std::shared_ptr<SpriteTexture> item) -> bool
        { return item.get() == sprite_texture; });

    this->kill_queue.push_back(*found);
  }

  void kill(const std::shared_ptr<SpriteTexture> &sprite_texture) override
  {
    this->kill(sprite_texture.get());
  }

private:
  void remove_dead_sprites()
  {
    for (const auto &sprite : this->kill_queue)
    {
      this->visible_sprites.remove(sprite);
      this->obstacle_sprites.remove(sprite);
      this->attack_sprites.remove(sprite);
      this->attackable_sprites.remove(sprite);
    }
    kill_queue.clear();
  }

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

  bool game_paused;
};

#endif
