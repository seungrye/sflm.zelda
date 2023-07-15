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
      weapon_index(0),
      weapon(WEAPON_DATA[weapon_index].first), can_switch_weapon(true),
      switch_duration_cooldown(sf::milliseconds(200))
{
  auto r = this->loadFromFile("./src/graphics/player.png");
  assert(r);
  this->get_rect({"topleft", pos});
  this->hitbox_ = this->rect_.inflate(0, -26);

  this->import_player_assets();
  // this->create_attack = ;
  // this->destroy_attack = ;
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
  }

  // magic
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
  {
    this->attacking = true;
    this->attack_time.restart();
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
    this->weapon = WEAPON_DATA[weapon_index].first;
  }
}

void Player::cooldowns()
{
  if (this->attacking)
  {
    if (this->attack_time.getElapsedTime() > this->attack_cooldown)
    {
      this->attacking = true;
    }
  }

  if (!this->can_switch_weapon)
  {
    if (this->weapon_switch_time.getElapsedTime() >
        this->switch_duration_cooldown)
    {
      this->can_switch_weapon = true;
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
}
void Player::update()
{
  this->input();
  this->cooldowns();
  this->get_status();
  this->animate();
  this->move(this->speed);
}