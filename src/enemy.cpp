#include "enemy.hpp"
#include <experimental/filesystem>

Enemy::Enemy(const std::string &monster_name, const sf::Vector2f &pos,
             const std::vector<std::shared_ptr<SpriteTexture>> &obstacle_sprites)
    : sprite_type("enemy"), status("idle"), Entity(0, 0.15, {0, 0}, obstacle_sprites),
      monster_name(monster_name), can_attack(true), attack_cooldown(sf::milliseconds(400)),
      vulernable(true), invincibility_duration(sf::milliseconds(300))
{
    this->import_graphics(monster_name);

    auto animation = this->animations[this->status];
    this->update_sprite(animation[static_cast<int>(this->frame_index)]);
    this->get_rect({"topleft", pos});
    this->hitbox_ = this->rect_.inflate(0, -26);
}

void Enemy::import_graphics(const std::string &monster_name)
{
    auto base_path = std::experimental::filesystem::path("./src/graphics/monsters");
    this->animations = {
        {"idle", import_folder(base_path / this->monster_name / "idle")},
        {"move", import_folder(base_path / this->monster_name / "move")},
        {"attack", import_folder(base_path / this->monster_name / "attack")}};
}