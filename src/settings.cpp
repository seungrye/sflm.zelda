#include "settings.hpp"
#include <string>
#include <vector>

std::vector<std::pair<std::string, int>> HITBOX_OFFSET = {
    {"player", -26},
    {"object", -40},
    {"grass", -10},
    {"invisible", 0},
};

inline std::pair<std::string, WeaponData> WP(const std::string &NAME, int COOLDOWN,
                                             int DAMAGE, const std::string &GRAPHIC)
{
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

inline std::pair<std::string, MagicData> MG(const std::string &NAME, int STRENGTH, int COST, const std::string &GRAPHIC)
{
    return std::make_pair(
        NAME,
        MagicData{.strength = STRENGTH, .cost = COST, .graphic = GRAPHIC});
}

std::vector<std::pair<std::string, MagicData>> MAGIC_DATA = {
    MG("flame", 5, 20, "./graphics/particles/flame/fire.png"),
    MG("heal", 5, 10, "./graphics/particles/heal/heal.png")};

inline std::pair<std::string, MonsterData> MON(const std::string &NAME,
                                               int HEALTH,
                                               int EXP,
                                               int DAMAGE,
                                               const std::string &ATTACK_TYPE,
                                               const std::string &ATTACK_SOUND,
                                               int SPEED,
                                               int RESISTANCE,
                                               int ATTACK_RADIUS,
                                               int NOTICE_RADIUS)
{
    return std::make_pair(
        NAME, MonsterData{.health = HEALTH, .exp = EXP, .damage = DAMAGE, .attack_type = ATTACK_TYPE, .attack_sound = ATTACK_SOUND, .speed = SPEED, .resistance = RESISTANCE, .attack_radius = ATTACK_RADIUS, .notice_radius = NOTICE_RADIUS});
}

// Note. resistance 는 사용자가 공격시 적이 밀려나는 정도를 의미함
std::vector<std::pair<std::string, MonsterData>> MONSTER_DATA = {
    MON("squid", 100, 100, 20, "slash", "./audio/attack/slash.wav", 3, 3, 80, 360),
    MON("raccoon", 300, 250, 40, "claw", "./audio/attack/claw.wav", 2, 3, 120, 400),
    MON("spirit", 100, 110, 8, "thunder", "./audio/attack/fireball.wav", 4, 3, 60, 350),
    MON("bamboo", 70, 120, 6, "leaf_attack", "./audio/attack/slash.wav", 3, 3, 50, 300)};