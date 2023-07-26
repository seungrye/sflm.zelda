#include "player.hpp"
#include "settings.hpp"
#include "support.hpp"
#include "upgrade.hpp"
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <algorithm>
#include <experimental/filesystem>

#include <cassert>
#include <memory>
#include <sstream>
#include <vector>
#include <functional>

Player::Player(const sf::Vector2f &pos,
               const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
               const std::function<void()> &create_attack,
               const std::function<void()> &destroy_attack,
               const std::function<void(const std::string &, int, int)> &create_magic,
               const std::function<void()> &destroy_magic,
               const std::function<void(int, const std::string &)> &damage_player)
    : Entity(0, 0.15f, {0, 0}, obstacle_sprites),
      status_("down"),
      speed(5),
      attacking(false),
      attack_cooldown(sf::milliseconds(400)),
      weapon_index_(0),
      magic_index_(0),
      can_switch_weapon_(true),
      switch_duration_cooldown(sf::milliseconds(200)),
      can_switch_magic_(true),
      vulernable_(true),
      invincibility_duration(sf::milliseconds(300)),
      create_attack(create_attack),
      destroy_attack(destroy_attack),
      create_magic(create_magic),
      destroy_magic(destroy_magic),
      damage_player_(damage_player)
{
  auto r = this->loadFromFile("./src/graphics/player.png");
  assert(r);
  this->get_rect({"topleft", pos});
  this->hitbox_ = this->rect_.inflate(0, -26);

  this->weapon_ = nth_name(WEAPON_DATA, this->weapon_index_);
  this->magic = nth_name(MAGIC_DATA, this->magic_index_);

  this->import_player_assets();

  this->stats_ = PLAYER_STATS;
  this->max_stats_ = PLAYER_MAX_STATS;
  this->upgrade_cost_ = UPGRADE_COST;
  this->health_ = this->stats_["health"];
  this->energy_ = this->stats_["energy"];
  this->exp_ = 0;

  this->weapon_attacks_sound_buffer.loadFromFile("./src/audio/sword.wav");
  this->weapon_attacks_sound.setBuffer(this->weapon_attacks_sound_buffer);
  this->weapon_attacks_sound.setVolume(10);
}

void Player::import_player_assets()
{
  auto animation_names = {
      "down", "down_attack", "down_idle", "left", "left_attack", "left_idle",
      "right", "right_attack", "right_idle", "up", "up_attack", "up_idle"};
  auto base_path = std::experimental::filesystem::path("./src/graphics/player");
  for (const auto &name : animation_names)
  {
    this->animations[name] = import_folder((base_path / name).string());
  }
}

void Player::input()
{
  if (this->attacking)
    return;

  // movement
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
  {
    this->direction.x = -1;
    this->status_ = "left";
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    this->direction.x = 1;
    this->status_ = "right";
  }
  else
    this->direction.x = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    this->direction.y = -1;
    this->status_ = "up";
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    this->direction.y = 1;
    this->status_ = "down";
  }
  else
    this->direction.y = 0;

  // attack
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
  {
    this->attacking = true;
    this->attack_time.restart();
    this->create_attack();
    this->weapon_attacks_sound.play();
  }

  // magic
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
  {
    this->attacking = true;
    this->attack_time.restart();
    auto magic_data = MAGIC_DATA[this->magic];
    this->create_magic(this->magic,
                       magic_data.strength,
                       magic_data.cost);
  }

  // swap weapon
  if (this->can_switch_weapon_ &&
      sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
  {
    this->can_switch_weapon_ = false;
    this->weapon_switch_time.restart();
    this->weapon_index_++;
    if (this->weapon_index_ >= WEAPON_DATA.size())
    {
      this->weapon_index_ = 0;
    }
    this->weapon_ = nth_name<WeaponData>(WEAPON_DATA, this->weapon_index_);
  }

  // swap magic::Z
  if (this->can_switch_magic_ && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
  {
    this->can_switch_magic_ = false;
    this->magic_switch_time.restart();
    this->magic_index_++;
    if (this->magic_index_ >= MAGIC_DATA.size())
    {
      this->magic_index_ = 0;
    }
    this->magic = nth_name(MAGIC_DATA, this->magic_index_);
  }
}

void Player::cooldowns()
{
  if (this->attacking)
  {
    if (this->attack_time.getElapsedTime() > (this->attack_cooldown + sf::milliseconds(WEAPON_DATA[this->weapon_].cooldown)))
    {
      this->attacking = false;
      this->destroy_attack();
      //?? destroy_magic 은 언제 어디서 호출되어야 할까?
    }
  }

  if (!this->can_switch_weapon_)
  {
    if (this->weapon_switch_time.getElapsedTime() > this->switch_duration_cooldown)
    {
      this->can_switch_weapon_ = true;
    }
  }

  if (!this->can_switch_magic_)
  {
    if (this->magic_switch_time.getElapsedTime() > this->switch_duration_cooldown)
    {
      this->can_switch_magic_ = true;
    }
  }

  if (!this->vulernable_)
  {
    if (this->hurt_time.getElapsedTime() > this->invincibility_duration)
    {
      this->vulernable_ = true;
    }
  }
}

void Player::get_status()
{
  if (this->direction.x == 0 && this->direction.y == 0)
  {
    if (std::string::npos == this->status_.find("idle") &&
        std::string::npos == this->status_.find("attack"))
    { // not idle or attack
      std::stringstream ss;
      ss << this->status_ << "_idle";
      this->status_ = ss.str();
    }
  }

  if (this->attacking)
  {
    this->direction.x = 0;
    this->direction.y = 0;
    if (std::string::npos == this->status_.find("attack"))
    {
      if (std::string::npos != this->status_.find("idle"))
      { // found
        auto index = this->status_.find("idle");
        this->status_ = this->status_.substr(0, index) + "attack";
      }
      else
      {
        this->status_ = this->status_ + "_attack";
      }
    }
  }
  else
  {
    if (std::string::npos != this->status_.find("attack"))
    { // found
      auto index = this->status_.find("attack");
      this->status_ = this->status_.substr(0, index) + "idle";
    }
  }
}

void Player::update_sprite(std::shared_ptr<SpriteTexture> sprite)
{
  this->texture_ = sprite->texture_; // 이걸 public 이 아닌 protected 로 바꿀수가 없을까?
  this->sprite_.setTexture(*this->texture_);
  this->rect_ = sprite->rect();
}

void Player::animate()
{
  auto animation = this->animations[this->status_];
  this->frame_index += this->animation_speed;
  if (this->frame_index >= static_cast<float>(animation.size()))
  {
    this->frame_index = 0;
  }

  this->update_sprite(animation[static_cast<int>(this->frame_index)]);
  this->get_rect({"center", this->hitbox_.center()});

  if (!this->vulernable_)
  { // make flicker
    auto alpha = this->wave_value();
    this->sprite_.setColor(sf::Color(0xff, 0xff, 0xff, alpha));
  }
  else
  {
    this->sprite_.setColor(sf::Color(0xff, 0xff, 0xff, 0xff));
  }
}
void Player::update()
{
  this->input();
  this->cooldowns();
  this->get_status();
  this->animate();
  this->move(this->stats_["speed"]);
  this->energy_recovery();
}

int Player::get_full_weapon_damage()
{
  auto base_damage = this->stats_["attack"];
  auto weapon_damage = WEAPON_DATA[this->weapon_].damage;
  return base_damage + weapon_damage;
}

int Player::get_full_magic_damage()
{
  auto base_damage = this->stats_["attack"];
  auto magic_damage = MAGIC_DATA[this->magic].strength;
  return base_damage + magic_damage;
}

void Player::energy_recovery()
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
