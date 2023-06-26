#ifndef __SETTINGS_HPP__
#define __SETTINGS_HPP__

#include <string>
#include <vector>

const int WIDTH = 1024;
const int HEIGHT = 768;
const int TILESIZE = 64;
const int FPS = 60;

struct WeaponData {
  int cooldown;
  int damage;
  std::string graphic;
};

extern std::vector<std::pair<std::string, WeaponData>> WEAPON_DATA;

#endif
