#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include "entity.hpp"
#include "player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <map>
#include <memory>

class Enemy : public Entity
{
public:
    Enemy(const std::string &monster_name, const sf::Vector2f &pos,
          const std::vector<std::shared_ptr<SpriteTexture>> &obstacle_sprites);

    void import_graphics(const std::string &monster_name);

    void animate()
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

    void update() override
    {
        // this->hit_reaction();
        this->move(this->speed);
        this->animate();
        // this->cooldowns();
        // this->check_death();
    }

    void enemy_update(std::shared_ptr<Player> player) {
        this->get_status(player);
        this->actions(player);
    }

private:
    void actions(std::shared_ptr<Player> player) {
        if (!this->status.compare("attack")) {
            // this->attack_sound.play();
            // this->attack_time = ;
            // player->damage_player(this->damage, this->attack_type);
        } else if (!this->status.compare("move")) {
            this->direction = this->get_player_distance_direction(player).second;
        } else {
            this->direction = py::Vector2f(0, 0);
        }
    }

    std::pair<float,py::Vector2f> get_player_distance_direction(std::shared_ptr<Player> player) {
        auto enemy_vec = py::Vector2f(this->rect_.center());
        auto player_rect = player->rect();
        auto player_vec = py::Vector2f(player_rect.center());
        auto distance = enemy_vec.distance_to(player_vec);
        auto direction = (distance > 0) ? py::Vector2f(player_vec - enemy_vec).normalize() : py::Vector2f(0, 0);
        return {distance, direction};
    }
    void get_status(std::shared_ptr<Player> player) {
        auto distance = this->get_player_distance_direction(player).first;
        if (this->can_attack && distance < this->attack_radius) {
            if (this->status.compare("attack")) {
                this->frame_index = 0;
            }
            this->status = "attack";
        } else if (distance < this->notice_radius) {
            this->status = "move";
        } else {
            this->status = "idle";
        }
    }

    void update_sprite(std::shared_ptr<SpriteTexture> sprite)
    {
        this->texture_ = sprite->texture_; // 이걸 public 이 아닌 protected 로 바꿀수가 없을까?
        this->sprite_.setTexture(*this->texture_);
        this->rect_ = sprite->rect();
    }

private:
    std::string status;
    // image
    std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> animations;
    std::string monster_name;
    int health;
    int exp;
    int damage;
    std::string attack_type;
    float speed;
    int resistance;
    int attack_radius;
    int notice_radius;
    bool can_attack;
    sf::Clock attack_time;
    sf::Time attack_cooldown;
    bool vulernable;
    sf::Clock hit_time;
    sf::Time invincibility_duration;
    // death_sound
    // hit_sound
    // attack_sound;

};

#endif /* __ENEMY_HPP__ */
