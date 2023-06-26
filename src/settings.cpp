#include "settings.hpp"
#include <string>
#include <vector>

inline std::pair<std::string, WeaponData> WP(std::string NAME, int COOLDOWN,
                                             int DAMAGE, std::string GRAPHIC) {
  return std::make_pair(
      NAME,
      WeaponData{.cooldown = COOLDOWN, .damage = DAMAGE, .graphic = GRAPHIC});
}

std::vector<std::pair<std::string, WeaponData>> WEAPON_DATA = {
    WP("sword", 100, 15, "./graphics/weapons/sword/full.png"),
    WP("axe", 100, 15, "./graphics/weapons/axe/full.png"),
    WP("lance", 100, 15, "./graphics/weapons/lance/full.png"),
    WP("rapier", 100, 15, "./graphics/weapons/rapier/full.png"),
    WP("sai", 100, 15, "./graphics/weapons/sai/full.png")};
