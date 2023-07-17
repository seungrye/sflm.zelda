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

struct Stats {
  int health;
  int energy;
  int attack;
  int magic;
  int speed;
};

class Player : public Entity {
public:
  Player(const sf::Vector2f &pos,
         const std::vector<std::shared_ptr<SpriteTexture>>& obstacle_sprites);

  void update() override;
  const int health() { return this->health_; }
  const int energy() { return this->energy_; }
  const Stats& stats() { return this->stats_; }
  const int exp() { return this->exp_; }
  const int weapon_index() { return this->weapon_index_; }
  const int magic_index() { return this->magic_index_; }
  const bool can_switch_weapon() { return this->can_switch_weapon_; }
  const bool can_switch_magic() { return this->can_switch_magic_; }

private:
  void import_player_assets();
  void input();
  void cooldowns();
  void get_status();
  void animate();
  void update_sprite(std::shared_ptr<SpriteTexture> sprite);

  void energy_recovery() {
    if (this->energy_ <= this->stats_.energy) {
      this->energy_ += 0.01 * this->stats_.magic;
    } else {
      this->energy_ = this->stats_.energy;
    }
  }

private:
  std::string status;
  int speed;
  bool attacking;
  sf::Time attack_cooldown;
  sf::Clock attack_time;
  // create_attack;
  // destroy_attack;
  int weapon_index_;
  std::string weapon;
  bool can_switch_weapon_;
  sf::Clock weapon_switch_time;
  sf::Clock magic_switch_time;
  sf::Time switch_duration_cooldown;

  int magic_index_;
  std::string magic;
  bool can_switch_magic_;

  bool vulernable;
  sf::Clock hurt_time;
  sf::Time invincibility_duration;

  std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> animations;

  Stats stats_;
  Stats max_stats;
  Stats upgrade_cost;
  int health_;
  int energy_;
  int exp_;

  // weapon_attacks_sound
};

#endif
