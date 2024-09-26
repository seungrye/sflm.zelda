#ifndef __ENEMY_HPP__
#define __ENEMY_HPP__

#include "pygame_adapter.hpp"
#include "support.hpp"
#include "entity.hpp"
#include "player.hpp"
#include "sprite_manager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <memory>
#include <functional>

std::pair<float, py::Vector2f> get_object_distance_direction(
    py::Rect<float> object,
    py::Rect<float> target);

class EnemyState
{
public: // interface
    EnemyState(const std::string &name) : name(name) {}
    virtual ~EnemyState() {}
    virtual void action() = 0;

public: // implement (can overridable)
    virtual const std::string &get() { return name; }
    virtual bool eq(const std::string &name) { return !this->name.compare(name); }

protected:
    const std::string name;
};

class EnemyIdleState : public EnemyState
{
public:
    EnemyIdleState(
        py::Vector2f &direction) : EnemyState("idle"), direction(direction) {}
    void action() override
    {
        this->direction = py::Vector2f(0, 0);
    }

private:
    py::Vector2f &direction;
};

class EnemyAttackState : public EnemyState
{
public:
    EnemyAttackState(
        std::shared_ptr<Player> player,
        sf::Clock &attack_time,
        sf::Sound& attack_sound,
        int damage,
        const std::string &attack_type) : EnemyState("attack"),
                                          player(player),
                                          attack_time(attack_time),
                                          damage(damage),
                                          attack_type(attack_type),
                                          attack_sound(attack_sound)
    {
    }
    void action() override
    {
        this->attack_sound.play();
        this->attack_time.restart();
        player->damage_player(this->damage, this->attack_type);
    }

private:
    std::shared_ptr<Player> player;
    sf::Sound& attack_sound;
    sf::Clock &attack_time;
    int damage;
    std::string attack_type;
};

class EnemyMoveState : public EnemyState
{
public:
    EnemyMoveState(
        py::Vector2f &direction,
        py::Rect<float> &rect_,
        std::shared_ptr<Player> player

        ) : EnemyState("move"),
            direction(direction),
            player(player),
            rect_(rect_)
    {
    }
    void action() override
    {
        this->direction = get_object_distance_direction(
                              this->rect_, this->player->rect())
                              .second;
    }

private:
    std::shared_ptr<Player> player;

    py::Vector2f &direction;
    py::Rect<float> &rect_;
};

class Enemy : public Entity
{
public:
    Enemy(const std::string &monster_name,
          const sf::Vector2f &pos,
          const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
          const std::function<void(const sf::Vector2f, const std::string &)> &trigger_death_particles,
          const std::function<void(int)> &add_exp,
          SpriteManager &sprite_manager,
          std::shared_ptr<Player> player);
    void import_graphics(const std::string &monster_name);
    void animate();
    void update() override;
    void get_damage(std::shared_ptr<Player> player, const std::string &attack_type);

private:
    void hit_reaction();
    void check_death();
    void cooldowns();
    void actions(std::shared_ptr<Player> player);
    std::pair<float, py::Vector2f> get_player_distance_direction(std::shared_ptr<Player> player);
    void get_status(std::shared_ptr<Player> player);
    void update_sprite(const sf::Texture &texture);

private:
    std::function<void(const sf::Vector2f, const std::string &)> trigger_death_particles;
    std::function<void(int)> add_exp;

private:
    std::shared_ptr<EnemyState> status;
    std::shared_ptr<Player> player;
    // image
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

    sf::SoundBuffer death_sound_buffer;
    sf::Sound death_sound;

    sf::SoundBuffer hit_sound_buffer;
    sf::Sound hit_sound;

    sf::SoundBuffer attack_sound_buffer;
    sf::Sound attack_sound;

    SpriteManager &sprite_manager;
};

#endif /* __ENEMY_HPP__ */
