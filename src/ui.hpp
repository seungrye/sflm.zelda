#ifndef __UI_HPP__
#define __UI_HPP__

#include "pygame_adapter.hpp"
#include "player.hpp"
#include "support.hpp"
#include "settings.hpp"
#include <memory>

class UI
{
public:
    UI() {}
    void display(std::shared_ptr<Player> player)
    {
        this->show_bar(player->health(), player->stats().health, this->health_bar_rect, HEALTH_COLOR);
        this->show_bar(player->energy(), player->stats().energy, this->energy_bar_rect, ENERGY_COLOR);
    }

private:
    void show_bar(int current, int max_amount, const py::Rect<float> &bg_rect, const sf::Color &color)
    {
    }

private:
    sf::Font font;
    py::Rect<float> health_bar_rect;
    py::Rect<float> energy_bar_rect;
    std::vector<std::shared_ptr<SpriteTexture>> weapon_graphics;
    std::vector<std::shared_ptr<SpriteTexture>> magic_graphics;
};

#endif /* __UI_HPP__ */
