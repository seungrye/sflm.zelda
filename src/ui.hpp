#ifndef __UI_HPP__
#define __UI_HPP__

#include "pygame_adapter.hpp"
#include "player.hpp"
#include "support.hpp"
#include "settings.hpp"
#include "game_window.hpp"
#include <memory>
#include <sstream>

class UI
{
public:
    UI();
    void display(std::shared_ptr<Player> player);

private:
    const py::Rect<float> selection_box(float left, float top, bool has_switched);
    void weapon_overlay(int weapon_index, bool has_switched);
    void magic_overlay(int magic_index, bool has_switched);
    void show_exp(int exp);
    void show_bar(float current, float max_amount, const py::Rect<float> &bg_rect, const sf::Color &color);

private:
    sf::Font font;
    sf::Text text;
    py::Rect<float> health_bar_rect;
    py::Rect<float> energy_bar_rect;
    std::vector<std::shared_ptr<SpriteTexture>> weapon_graphics;
    std::vector<std::shared_ptr<SpriteTexture>> magic_graphics;
};

#endif /* __UI_HPP__ */
