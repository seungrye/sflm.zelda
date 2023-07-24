#include "enemy.hpp"
#include "settings.hpp"
#include <experimental/filesystem>
#include <algorithm>

Enemy::Enemy(const std::string &monster_name, const sf::Vector2f &pos,
             const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites)
    : status("idle"),
      Entity(0, 0.15, {0, 0}, obstacle_sprites),
      monster_name(monster_name),
      can_attack(true),
      attack_cooldown(sf::milliseconds(400)),
      vulernable(true),
      invincibility_duration(sf::milliseconds(300))
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

std::pair<float, py::Vector2f> Enemy::get_player_distance_direction(std::shared_ptr<Player> player)
{
    auto enemy_vec = py::Vector2f(this->rect_.center());
    auto player_rect = player->rect();
    auto player_vec = py::Vector2f(player_rect.center());
    auto distance = enemy_vec.distance_to(player_vec);
    auto direction = (distance > 0) ? py::Vector2f(player_vec - enemy_vec).normalize() : py::Vector2f(0, 0);
    return {distance, direction};
}

void Enemy::get_status(std::shared_ptr<Player> player)
{
    auto distance = this->get_player_distance_direction(player).first;
    if (this->can_attack && distance < this->attack_radius)
    {
        if (this->status.compare("attack"))
        {
            this->frame_index = 0;
        }
        this->status = "attack";
    }
    else if (distance < this->notice_radius)
    {
        this->status = "move";
    }
    else
    {
        this->status = "idle";
    }
}

void Enemy::actions(std::shared_ptr<Player> player)
{
    if (!this->status.compare("attack"))
    {
        // this->attack_sound.play();
        this->attack_time.restart();
        // player->damage_player(this->damage, this->attack_type);
    }
    else if (!this->status.compare("move"))
    {
        this->direction = this->get_player_distance_direction(player).second;
    }
    else
    {
        this->direction = py::Vector2f(0, 0);
    }
}

void Enemy::animate()
{
    auto animation = this->animations[this->status];

    this->frame_index += this->animation_speed;
    if (this->frame_index >= static_cast<float>(animation.size()))
    {
        if (!this->status.compare("attack"))
        {
            this->can_attack = false;
        }
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

void Enemy::update()
{
    this->hit_reaction();
    this->move(this->speed);
    this->animate();
    this->cooldowns();
    this->check_death();
}

void Enemy::enemy_update(std::shared_ptr<Player> player)
{
    this->get_status(player);
    this->actions(player);
}

void Enemy::get_damage(std::shared_ptr<Player> player, const std::string &attack_type)
{
    if (this->vulernable)
    {
        // this->hit_sound.play();
        this->direction = this->get_player_distance_direction(player).second;
        if (!attack_type.compare("weapon"))
        {
            this->health -= player->get_full_weapon_damage();
        }
        else
        {
            this->health -= player->get_full_magic_damage();
        }
        this->hit_time.restart();
        this->vulernable = false;
    }
}

void Enemy::hit_reaction()
{
    if (!this->vulernable)
    {
        this->direction.x *= (this->resistance * -1);
        this->direction.y *= (this->resistance * -1);
    }
}

void Enemy::check_death()
{
    if (this->health <= 0)
    {
        // this->death_sound.play();
        // this->trigger_death_particles(this->rect_.center(), this->monster_name);
        // this->kill(); // remove the Sprite from all Groups
        // this->add_exp(this->exp);
    }
}

void Enemy::cooldowns()
{
    if (!this->can_attack)
    {
        if (this->attack_time.getElapsedTime() > this->attack_cooldown)
        {
            this->can_attack = true;
        }
    }

    if (!this->vulernable)
    {
        if (this->hit_time.getElapsedTime() > this->invincibility_duration)
        {
            this->vulernable = true;
        }
    }
}

void Enemy::update_sprite(std::shared_ptr<SpriteTexture> sprite)
{
    this->texture_ = sprite->texture_; // 이걸 public 이 아닌 protected 로 바꿀수가 없을까?
    this->sprite_.setTexture(*this->texture_);
    this->rect_ = sprite->rect();
}