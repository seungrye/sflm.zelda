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
#include <map>
#include <memory>
#include <functional>

class Enemy : public Entity
{
public:
    Enemy(const std::string &monster_name,
          const sf::Vector2f &pos,
          const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
          const std::function<void(const sf::Vector2f, const std::string &)> &trigger_death_particles,
          const std::function<void(int)> &add_exp,
          SpriteManager &sprite_manager);
    void import_graphics(const std::string &monster_name);
    void animate();
    void update() override;
    void enemy_update(std::shared_ptr<Player> player);
    void get_damage(std::shared_ptr<Player> player, const std::string &attack_type);

private:
    void hit_reaction();
    void check_death();
    void cooldowns();
    void actions(std::shared_ptr<Player> player);
    std::pair<float, py::Vector2f> get_player_distance_direction(std::shared_ptr<Player> player);
    void get_status(std::shared_ptr<Player> player);
    void update_sprite(const sf::Texture& texture);

private:
    std::function<void(const sf::Vector2f, const std::string &)> trigger_death_particles;
    std::function<void(int)> add_exp;

private:
    std::string status;
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
