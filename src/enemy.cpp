#include "enemy.hpp"
#include "settings.hpp"
#include <experimental/filesystem>
#include <algorithm>

std::pair<float, py::Vector2f> get_object_distance_direction(
    py::Rect<float> object,
    py::Rect<float> target)
{
    auto object_vec = py::Vector2f(object.center());
    auto target_vec = py::Vector2f(target.center());
    auto distance = object_vec.distance_to(target_vec);
    auto direction = (distance > 0) ? py::Vector2f(target_vec - object_vec).normalize() : py::Vector2f(0, 0);
    return {distance, direction};
}

Enemy::Enemy(const std::string &monster_name,
             const sf::Vector2f &pos,
             const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
             const std::function<void(const sf::Vector2f, const std::string &)> &trigger_death_particles,
             const std::function<void(int)> &add_exp,
             SpriteManager &sprite_manager,
             std::shared_ptr<Player> player)
    : Entity(0, 0.15, {0, 0}, obstacle_sprites),
      monster_name(monster_name),
      can_attack(true),
      attack_cooldown(sf::milliseconds(400)),
      vulernable(true),
      invincibility_duration(sf::milliseconds(300)),
      trigger_death_particles(trigger_death_particles),
      add_exp(add_exp),
      sprite_manager(sprite_manager),
      player(player)
{
    this->sprite_type_ = "enemy";
    this->import_graphics(monster_name);
    this->status = std::make_shared<EnemyIdleState>(this->direction);

    auto name = monster_name + "/" + this->status->get();
    auto &animation = this->sprite_manager.textures(name);
    this->update_sprite(animation[static_cast<int>(this->frame_index)]);
    // set object rectangle
    auto rect = this->sprite_.getTextureRect();
    this->rect_ = py::Rect<float>(rect.left, rect.top, rect.width, rect.height);
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

    this->death_sound_buffer.loadFromFile("./src/audio/hit.wav");
    this->death_sound.setBuffer(this->death_sound_buffer);
    this->death_sound.setVolume(10);

    this->hit_sound_buffer.loadFromFile("./src/audio/hit.wav");
    this->hit_sound.setBuffer(this->hit_sound_buffer);
    this->hit_sound.setVolume(10);

    this->attack_sound_buffer.loadFromFile(monster_info->second.attack_sound);
    this->attack_sound.setBuffer(attack_sound_buffer);
    this->attack_sound.setVolume(10);
}

void Enemy::import_graphics(const std::string &monster_name)
{
    auto base_path = std::experimental::filesystem::path("./src/graphics/monsters");
    sprite_manager.import(this->monster_name + "/idle", base_path / this->monster_name / "idle");
    sprite_manager.import(this->monster_name + "/move", base_path / this->monster_name / "move");
    sprite_manager.import(this->monster_name + "/attack", base_path / this->monster_name / "attack");
}

void Enemy::get_status(std::shared_ptr<Player> player)
{
    auto distance = get_object_distance_direction(this->rect_, player->rect()).first;
    if (this->can_attack && distance < this->attack_radius)
    {
        if (!this->status->eq("attack"))
        {
            this->frame_index = 0;
            this->status = std::make_shared<EnemyAttackState>(
                this->player,
                this->attack_time,
                this->attack_sound,
                this->damage,
                this->attack_type);
        }
    }
    else if (distance < this->notice_radius)
    {
        this->status = std::make_shared<EnemyMoveState>(
            this->direction,
            this->rect_,
            this->player);
    }
    else
    {
        if (!this->status->eq("idle"))
        {
            this->status = std::make_shared<EnemyIdleState>(
                this->direction);
        }
    }
}

void Enemy::actions(std::shared_ptr<Player> player)
{
    this->status->action();
}

void Enemy::animate()
{
    auto name = monster_name + "/" + this->status->get();
    auto &animation = this->sprite_manager.textures(name);

    this->frame_index += this->animation_speed;
    if (this->frame_index >= static_cast<float>(animation.size()))
    {
        if (this->status->eq("attack"))
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

    this->get_status(this->player);
    this->actions(this->player);
}

void Enemy::get_damage(std::shared_ptr<Player> player, const std::string &attack_type)
{
    if (this->vulernable)
    {
        this->hit_sound.play();
        this->direction = get_object_distance_direction(this->rect_, player->rect()).second;
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
        this->death_sound.play();
        this->trigger_death_particles(this->rect_.center(), this->monster_name);
        this->sprite_manager.kill(this); // remove the Sprite from all Groups
        this->add_exp(this->exp);
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

void Enemy::update_sprite(const sf::Texture &texture)
{
    this->sprite_.setTexture(texture);
}