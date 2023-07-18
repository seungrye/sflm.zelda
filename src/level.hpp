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

class YSortCameraGroup
{
public:
  YSortCameraGroup();
  void custom_draw(std::shared_ptr<Player> player);
  void update();
  void enemy_update(std::shared_ptr<Player> player)
  {
    for (const auto &sprite : this->sprites)
    {
      if (sprite->is("enemy"))
      {
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

class Level : public virtual IDamagePlayer,
              // public ITriggerDeathParticles,
              public virtual ICreateAttack,
              public virtual IDestroyAttack,
              // public IAddXp,
              public virtual ICreateMagic,
              public virtual IDestroyMagic
{
public:
  Level();
  void fire(IDamagePlayer * /*self*/) override
  {
    std::cout << "IDamagePlayer" << std::endl;
  }
  void fire(ICreateAttack * /*self*/) override
  {
    std::cout << "ICreateAttack" << std::endl;
  }
  void fire(IDestroyAttack * /*self*/) override
  {
    std::cout << "IDestroyAttack" << std::endl;
  }
  void fire(ICreateMagic * /*self*/, const std::string style, int strength, int cost) override
  {
    std::cout << "ICreateMagic(" << style << strength << cost << ")" << std::endl;
  }
  void fire(IDestroyMagic * /*self*/) override
  {
    std::cout << "IDestroyMagic" << std::endl;
  }

  void create_map();
  void create_attack();
  void destroy_attack();
  void run();
  void toggle_menu()
  {
    this->game_paused = !this->game_paused;
    // std::cout<<"toggle menu called"<<std::endl;
  }

private:
  void player_attack_logic();

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
  // std::shared_ptr<Weapon> current_attack;
  UI ui;
  std::shared_ptr<Upgrade> upgrade;
  std::shared_ptr<AnimationPlayer> animation_player;
  // std::shared_ptr<MagicPlayer> magic_player;

  bool game_paused;
};

#endif
