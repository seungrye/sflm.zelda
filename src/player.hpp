#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include "entity.hpp"
#include "settings.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <list>
#include <functional>

class Player : public Entity
{
public:
  Player(const sf::Vector2f &pos,
         const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
         const std::function<void()> &create_attack,
         const std::function<void()> &destroy_attack,
         const std::function<void(const std::string&, int, int)> &create_magic,
         const std::function<void()> &destroy_magic,
         const std::function<void(int, const std::string &)> &damage_player);

  void update() override;
  const int health() { return this->health_; }
  void health(int health) { this->health_ = health; }
  const int energy() { return this->energy_; }
  void energy(int energy) { this->energy_ = energy; }
  const std::map<std::string, int> &stats() { return this->stats_; }
  void stats(const std::string &key, int value) { this->stats_[key] = value; }
  const std::map<std::string, int> &max_stats() { return this->max_stats_; }
  const int exp() { return this->exp_; }
  const void exp(int exp) { this->exp_ = exp; }
  const int weapon_index() { return this->weapon_index_; }
  const int magic_index() { return this->magic_index_; }
  const bool can_switch_weapon() { return this->can_switch_weapon_; }
  const bool can_switch_magic() { return this->can_switch_magic_; }
  const int get_value_by_index(int index) { return nth_item(this->stats_, index).second; }
  const int get_cost_by_index(int index) { return nth_item(this->upgrade_cost_, index).second; }
  const std::map<std::string, int> &upgrade_cost() { return this->upgrade_cost_; }
  void upgrade_cost(const std::string &key, int value) { this->upgrade_cost_[key] = value; }
  const std::string &status() { return this->status_; }
  const std::string &weapon() { return this->weapon_; }
  int get_full_weapon_damage();
  int get_full_magic_damage();
  bool vulernable() { return this->vulernable_; }
  void vulernable(bool b) { this->vulernable_ = b; }
  void damage_player(int value, const std::string &attack_type) { return damage_player_(value, attack_type); }

private:
  void import_player_assets();
  void input();
  void cooldowns();
  void get_status();
  void animate();
  void update_sprite(std::shared_ptr<SpriteTexture> sprite);
  void energy_recovery();

private:
  std::function<void()> create_attack;
  std::function<void()> destroy_attack;
  std::function<void(const std::string&, int, int)> create_magic;
  std::function<void()> destroy_magic;
  std::function<void(int, const std::string &)> damage_player_;

private:
  std::string status_;
  int speed;
  bool attacking;
  sf::Time attack_cooldown;
  sf::Clock attack_time;
  int weapon_index_;
  std::string weapon_;
  bool can_switch_weapon_;
  sf::Clock weapon_switch_time;
  sf::Clock magic_switch_time;
  sf::Time switch_duration_cooldown;

  int magic_index_;
  std::string magic;
  bool can_switch_magic_;

  bool vulernable_;
  sf::Clock hurt_time;
  sf::Time invincibility_duration;

  std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> animations;

  std::map<std::string, int> stats_;
  std::map<std::string, int> max_stats_;
  std::map<std::string, int> upgrade_cost_;
  int health_;
  int energy_;
  int exp_;

  sf::SoundBuffer weapon_attacks_sound_buffer;
  sf::Sound weapon_attacks_sound;
};

#endif
