#include "enemy.hpp"
#include "settings.hpp"
#include <experimental/filesystem>
#include <algorithm>

Enemy::Enemy(const std::string &monster_name, const sf::Vector2f &pos,
             const std::vector<std::shared_ptr<SpriteTexture>> &obstacle_sprites)
    : status("idle"), Entity(0, 0.15, {0, 0}, obstacle_sprites),
      monster_name(monster_name), can_attack(true), attack_cooldown(sf::milliseconds(400)),
      vulernable(true), invincibility_duration(sf::milliseconds(300))
{
    this->sprite_type_ = "enemy";
    this->import_graphics(monster_name);
    this->status = "idle";

    auto animation = this->animations[this->status];
    this->update_sprite(animation[static_cast<int>(this->frame_index)]);
    this->get_rect({"topleft", pos});
    this->hitbox_ = this->rect_.inflate(0, -26);

    auto monster_info = std::find_if(MONSTER_DATA.begin(), MONSTER_DATA.end(),
                                     [&monster_name](const std::pair<std::string, MonsterData> &m)
                                     { return !m.first.compare(monster_name); });
    this->health = monster_info->second.health;
    this->exp = monster_info->second.exp;
    this->damage = monster_info->second.damage;
    this->attack_type = monster_info->second.attack_type;
    this->speed = monster_info->second.speed;
    this->resistance = monster_info->second.resistance;
    this->attack_radius = monster_info->second.attack_radius;
    this->notice_radius = monster_info->second.notice_radius;

    // this->death_sound = ;
    // this->hit_sound = ;
    // this->attack_sound = ;
}

void Enemy::import_graphics(const std::string &monster_name)
{
    auto base_path = std::experimental::filesystem::path("./src/graphics/monsters");
    this->animations = {
        {"idle", import_folder(base_path / this->monster_name / "idle")},
        {"move", import_folder(base_path / this->monster_name / "move")},
        {"attack", import_folder(base_path / this->monster_name / "attack")}};
}