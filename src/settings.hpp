#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

const int WIDTH = 1024;
const int HEIGHT = 768;
const int TILESIZE = 64;
const int FPS = 60;

extern std::vector<std::pair<std::string, int>> HITBOX_OFFSET;

const int BAR_HEIGHT = 20;
const int HEALTH_BAR_WIDTH = 200;
const int ENERGY_BAR_WIDTH = 140;
const int ITEM_BOX_SIZE = 80;
const std::string UI_FONT = "./graphics/font/joystix.ttf";
const int UI_FONT_SIZE = 18;

const auto WATER_COLOR = sf::Color(0x71, 0xdd, 0xee);
const auto UI_BG_COLOR = sf::Color(0x22, 0x22, 0x22);
const auto UI_BORDER_COLOR = sf::Color(0x11, 0x11, 0x11);
const auto TEXT_COLOR = sf::Color(0xee, 0xee, 0xee);

struct WeaponData {
  int cooldown;
  int damage;
  std::string graphic;
};

extern std::vector<std::pair<std::string, WeaponData>> WEAPON_DATA;

struct MagicData {
  int strength;
  int cost;
  std::string graphic;
};

extern std::vector<std::pair<std::string, MagicData>> MAGIC_DATA;

struct MonsterData {
  int health;
  int exp;
  int damage;
  std::string attack_type;
  std::string attack_sound;
  int speed;
  int resistance;
  int attack_radius;
  int notice_radius;
};

extern std::vector<std::pair<std::string, MonsterData>> MONSTER_DATA;

#endif
