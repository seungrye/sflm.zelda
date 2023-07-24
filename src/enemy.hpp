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
          const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites);
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
    void update_sprite(std::shared_ptr<SpriteTexture> sprite);

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
