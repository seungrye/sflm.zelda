#include "settings.hpp"
#include <string>
#include <cassert>

std::map<std::string, int> HITBOX_OFFSET = {
    {"player", -26},
    {"object", -40},
    {"grass", -10},
    {"invisible", 0},
};

std::map<std::string, WeaponData> WEAPON_DATA = {
    {"sword", WeaponData{.cooldown = 100, .damage = 15, .graphic = "./src/graphics/weapons/sword/full.png"}},
    {"axe", WeaponData{.cooldown = 100, .damage = 15, .graphic = "./src/graphics/weapons/axe/full.png"}},
    {"lance", WeaponData{.cooldown = 100, .damage = 15, .graphic = "./src/graphics/weapons/lance/full.png"}},
    {"rapier", WeaponData{.cooldown = 100, .damage = 15, .graphic = "./src/graphics/weapons/rapier/full.png"}},
    {"sai", WeaponData{.cooldown = 100, .damage = 15, .graphic = "./src/graphics/weapons/sai/full.png"}}};

std::map<std::string, MagicData> MAGIC_DATA = {
    {"flame", MagicData{.strength = 5, .cost = 20, .graphic = "./src/graphics/particles/flame/fire.png"}},
    {"heal", MagicData{.strength = 5, .cost = 10, .graphic = "./src/graphics/particles/heal/heal.png"}}};

// Note. resistance 는 사용자가 공격시 적이 밀려나는 정도를 의미함
std::map<std::string, MonsterData> MONSTER_DATA = {
    {"squid", MonsterData{.health = 100, .exp = 100, .damage = 20, .attack_type = "slash", .attack_sound = "./src/audio/attack/slash.wav", .speed = 3, .resistance = 3, .attack_radius = 80, .notice_radius = 360}},
    {"raccoon", MonsterData{.health = 300, .exp = 250, .damage = 40, .attack_type = "claw", .attack_sound = "./src/audio/attack/claw.wav", .speed = 2, .resistance = 3, .attack_radius = 120, .notice_radius = 400}},
    {"spirit", MonsterData{.health = 100, .exp = 110, .damage = 8, .attack_type = "thunder", .attack_sound = "./src/audio/attack/fireball.wav", .speed = 4, .resistance = 3, .attack_radius = 60, .notice_radius = 350}},
    {"bamboo", MonsterData{.health = 70, .exp = 120, .damage = 6, .attack_type = "leaf_attack", .attack_sound = "./src/audio/attack/slash.wav", .speed = 3, .resistance = 3, .attack_radius = 50, .notice_radius = 300}}};

std::map<std::string, float> PLAYER_STATS = {
    {"health", 100},
    {"energy", 60},
    {"attack", 10},
    {"magic", 4},
    {"speed", 6}};

std::map<std::string, float> PLAYER_MAX_STATS = {
    {"health", 300},
    {"energy", 140},
    {"attack", 20},
    {"magic", 10},
    {"speed", 12}};

std::map<std::string, float> UPGRADE_COST = {
    {"health", 100},
    {"energy", 100},
    {"attack", 100},
    {"magic", 100},
    {"speed", 100}};
