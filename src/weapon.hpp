#ifndef __WEAPON_HPP__
#define __WEAPON_HPP__

#include "support.hpp"
#include "player.hpp"
#include <sstream>

class Weapon : public SpriteTexture
{
public:
    Weapon(const std::shared_ptr<Player> &player)
    {
        this->sprite_type_ = "weapon";

        auto status = player->status();
        auto direction = status.substr(0, status.find('_'));
        std::stringstream full_path;
        full_path << "./src/graphics/weapons/" << player->weapon() << "/" << direction << ".png";

        auto r = this->loadFromFile(full_path.str());
        assert(r);

        auto player_rect = player->rect();
        if (!direction.compare("right"))
        {
            this->get_rect({"midleft", player_rect.midright() + sf::Vector2f(0, 16.f)});
        }
        else if (!direction.compare("left"))
        {
            this->get_rect({"midright", player_rect.midleft() + sf::Vector2f(0, 16.f)});
        }
        else if (!direction.compare("down"))
        {
            this->get_rect({"midtop", player_rect.midbottom() + sf::Vector2f(-10.f, 0)});
        }
        else
        {
            this->get_rect({"midbottom", player_rect.midtop() + sf::Vector2f(-10.f, 0)});
        }
    }

private:
};

#endif /* __WEAPON_HPP__ */
