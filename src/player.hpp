#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include "entity.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <memory>

class Player : public Entity {
public:
  Player(const sf::Vector2f &pos,
         const std::vector<std::shared_ptr<SpriteTexture>>& obstacle_sprites);

  void update();

private:
  void import_player_assets();
  void input();
  void cooldowns();
  void get_status();
  void animate();
  void update_sprite(std::shared_ptr<SpriteTexture> sprite);

private:
  std::string status;
  int speed;
  bool attacking;
  sf::Time attack_cooldown;
  sf::Clock attack_time;
  // create_attack;
  // destroy_attack;
  int weapon_index;
  std::string weapon;
  bool can_switch_weapon;
  sf::Clock weapon_switch_time;
  sf::Time switch_duration_cooldown;

  std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> animations;
};

#endif
