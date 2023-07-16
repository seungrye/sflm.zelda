#include "player.hpp"
#include "settings.hpp"
#include "support.hpp"
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

Player::Player(const sf::Vector2f &pos,
               const std::vector<std::shared_ptr<SpriteTexture>> &obstacle_sprites)
    : status("down"), Entity(0, 0.15f, {0, 0}, obstacle_sprites), speed(5),
      attacking(false), attack_cooldown(sf::milliseconds(400)),
      weapon_index(0), magic_index(0),
      can_switch_weapon(true),
      switch_duration_cooldown(sf::milliseconds(200)),
      can_switch_magic(true),
      vulernable(true), invincibility_duration(sf::milliseconds(300))
{
  auto r = this->loadFromFile("./src/graphics/player.png");
  assert(r);
  this->get_rect({"topleft", pos});
  this->hitbox_ = this->rect_.inflate(0, -26);

  this->weapon = nth_name(WEAPON_DATA, this->weapon_index);
  this->magic = nth_name(MAGIC_DATA, this->magic_index);

  this->import_player_assets();
  // this->create_attack = ;
  // this->destroy_attack = ;

  this->stats_ = {.health = 100, .energy = 60, .attack = 10, .magic = 4, .speed = 6};
  this->max_stats = {.health = 300, .energy = 140, .attack = 20, .magic = 10, .speed = 12};
  this->upgrade_cost = {.health = 100, .energy = 100, .attack = 100, .magic = 100, .speed = 100};
  this->health_ = this->stats_.health;
  this->energy_ = this->stats_.energy;
  this->exp = 0;
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
    this->status = "left";
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
  {
    this->direction.x = 1;
    this->status = "right";
  }
  else
    this->direction.x = 0;

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
  {
    this->direction.y = -1;
    this->status = "up";
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
  {
    this->direction.y = 1;
    this->status = "down";
  }
  else
    this->direction.y = 0;

  // attack
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
  {
    this->attacking = true;
    this->attack_time.restart();
    // this->create_attack();
    // this->weapon_attacks_sound.play();
  }

  // magic
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
  {
    this->attacking = true;
    this->attack_time.restart();
    // this->create_magic(this->magic, MAGIC_DATA[])
  }

  // swap weapon
  if (this->can_switch_weapon &&
      sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
  {
    this->can_switch_weapon = false;
    this->weapon_switch_time.restart();
    this->weapon_index++;
    if (this->weapon_index >= WEAPON_DATA.size())
    {
      this->weapon_index = 0;
    }
    this->weapon = nth_name<WeaponData>(WEAPON_DATA, this->weapon_index);
  }

  // swap magic
  if (this->can_switch_magic && sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
  {
    this->can_switch_magic = false;
    this->magic_index++;
    if (this->magic_index >= MAGIC_DATA.size())
    {
      this->magic_index = 0;
    }
    this->magic = nth_name(MAGIC_DATA, this->magic_index);
  }
}

void Player::cooldowns()
{
  if (this->attacking)
  {
    if (this->attack_time.getElapsedTime() > (this->attack_cooldown + sf::milliseconds(WEAPON_DATA[this->weapon].cooldown)))
    {
      this->attacking = true;
      // this->destory_attack()
    }
  }

  if (!this->can_switch_weapon)
  {
    if (this->weapon_switch_time.getElapsedTime() > this->switch_duration_cooldown)
    {
      this->can_switch_weapon = true;
    }
  }

  if (!this->vulernable)
  {
    if (this->hurt_time.getElapsedTime() > this->invincibility_duration)
    {
      this->vulernable = true;
    }
  }
}

void Player::get_status()
{
  if (this->direction.x == 0 && this->direction.y == 0)
  {
    if (std::string::npos == this->status.find("idle") &&
        std::string::npos == this->status.find("attack"))
    { // not idle or attack
      std::stringstream ss;
      ss << this->status << "_idle";
      this->status = ss.str();
    }
  }

  if (this->attacking)
  {
    this->direction.x = 0;
    this->direction.y = 0;
    if (std::string::npos == this->status.find("attack"))
    {
      if (std::string::npos != this->status.compare("idle"))
      { // found
        auto index = this->status.find_first_of("idle");
        this->status = this->status.substr(0, index) + "attack";
      }
      else
      {
        this->status = this->status + "_attack";
      }
    }
  }
  else
  {
    if (std::string::npos != this->status.find("attack"))
    { // found
      auto index = this->status.find_first_of("attack");
      this->status = this->status.substr(0, index) + "idle";
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
  auto animation = this->animations[this->status];
  this->frame_index += this->animation_speed;
  if (this->frame_index >= static_cast<float>(animation.size()))
  {
    this->frame_index = 0;
  }

  this->update_sprite(animation[static_cast<int>(this->frame_index)]);
  this->get_rect({"center", this->hitbox_.center()});

  if (!this->vulernable)
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
  this->move(this->stats_.speed);
  this->energy_recovery();
}