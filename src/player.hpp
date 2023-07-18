#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include "entity.hpp"
#include "settings.hpp"
#include "actions.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <memory>

class Player : public Entity
{
public:
  Player(const sf::Vector2f &pos,
         const std::vector<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
         ICreateAttack *create_attack,
         IDestroyAttack *destroy_attack,
         ICreateMagic *create_magic,
         IDestroyMagic *destroy_magic,
         IDamagePlayer *damage_player);

  void update() override;
  const int health() { return this->health_; }
  const int energy() { return this->energy_; }
  const std::map<std::string, int> &stats() { return this->stats_; }
  void stats(const std::string &key, int value)
  {
    this->stats_[key] = value;
  }
  const std::map<std::string, int> &max_stats() { return this->max_stats_; }
  const int exp() { return this->exp_; }
  const void exp(int exp) { this->exp_ = exp; }
  const int weapon_index() { return this->weapon_index_; }
  const int magic_index() { return this->magic_index_; }
  const bool can_switch_weapon() { return this->can_switch_weapon_; }
  const bool can_switch_magic() { return this->can_switch_magic_; }
  const int get_value_by_index(int index)
  {
    auto item = nth_item(this->stats_, index);
    return item.second;
  }
  const int get_cost_by_index(int index)
  {
    auto item = nth_item(this->upgrade_cost_, index);
    return item.second;
  }
  const std::map<std::string, int> &upgrade_cost()
  {
    return this->upgrade_cost_;
  }
  void upgrade_cost(const std::string &key, int value)
  {
    this->upgrade_cost_[key] = value;
  }

    int get_full_weapon_damage() {
        auto base_damage = this->stats_["attack"];
        auto weapon_damage = WEAPON_DATA[this->weapon].damage;
        return base_damage + weapon_damage;
    }

    int get_full_magic_damage() {
        auto base_damage = this->stats_["attack"];
        auto magic_damage = MAGIC_DATA[this->magic].strength;
        return base_damage + magic_damage;
    }

private:
  void import_player_assets();
  void input();
  void cooldowns();
  void get_status();
  void animate();
  void update_sprite(std::shared_ptr<SpriteTexture> sprite);

  void energy_recovery()
  {
    if (this->energy_ <= this->stats_["energy"])
    {
      this->energy_ += 0.01 * this->stats_["magic"];
    }
    else
    {
      this->energy_ = this->stats_["energy"];
    }
  }

private:
  ICreateAttack *create_attack;
  IDestroyAttack *destroy_attack;
  ICreateMagic *create_magic;
  IDestroyMagic *destroy_magic;
  IDamagePlayer *damage_player;

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

  std::map<std::string, int> stats_;
  std::map<std::string, int> max_stats_;
  std::map<std::string, int> upgrade_cost_;
  int health_;
  int energy_;
  int exp_;

  // weapon_attacks_sound
};

#endif
