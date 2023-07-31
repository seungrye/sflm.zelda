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
    UI()
        : health_bar_rect(10, 10, HEALTH_BAR_WIDTH, BAR_HEIGHT),
          energy_bar_rect(10, 20 + BAR_HEIGHT, ENERGY_BAR_WIDTH, BAR_HEIGHT)
    {
        auto r = this->font.loadFromFile(UI_FONT);
        assert(r);

        this->text.setFont(this->font);
        this->text.setCharacterSize(UI_FONT_SIZE);

        for (const auto &item : WEAPON_DATA)
        {
            auto path = item.second.graphic;
            this->weapon_graphics.push_back(std::make_shared<SpriteTexture>(path));
        }

        for (const auto &item : MAGIC_DATA)
        {
            auto path = item.second.graphic;
            this->magic_graphics.push_back(std::make_shared<SpriteTexture>(path));
        }
    }
    void display(std::shared_ptr<Player> player)
    {
        auto stats = player->stats();
        this->show_bar(player->health(), stats["health"], this->health_bar_rect, HEALTH_COLOR);
        this->show_bar(player->energy(), stats["energy"], this->energy_bar_rect, ENERGY_COLOR);
        this->show_exp(player->exp());

        this->weapon_overlay(player->weapon_index(), player->can_switch_weapon());
        this->magic_overlay(player->magic_index(), player->can_switch_magic());
    }

private:
    const py::Rect<float> selection_box(float left, float top, bool has_switched)
    {
        sf::RenderTexture &renderTexture = GameWindow::instance().screen();

        auto bg_rect = py::Rect<float>(left, top, ITEM_BOX_SIZE, ITEM_BOX_SIZE);

        sf::RectangleShape rectangle({bg_rect.width, bg_rect.height});
        rectangle.setFillColor(UI_BG_COLOR);
        rectangle.setPosition({bg_rect.left, bg_rect.top});

        sf::RectangleShape border({bg_rect.width, bg_rect.height});
        border.setOutlineThickness(3);
        border.setFillColor(sf::Color::Transparent);
        border.setPosition({bg_rect.left, bg_rect.top});
        if (has_switched)
        {
            border.setOutlineColor(UI_BORDER_COLOR_ACTIVE);
        }
        else
        {
            border.setOutlineColor(UI_BORDER_COLOR);
        }

        renderTexture.draw(rectangle);
        renderTexture.draw(border);

        return bg_rect;
    }

    void weapon_overlay(int weapon_index, bool has_switched)
    {
        sf::RenderTexture &renderTexture = GameWindow::instance().screen();

        auto bg_rect = this->selection_box(10, 550, has_switched);
        auto weapon_sprite = this->weapon_graphics[weapon_index];
        weapon_sprite->set_position({"center", bg_rect.center()});
        renderTexture.draw(weapon_sprite->surf());
    }

    void magic_overlay(int magic_index, bool has_switched)
    {
        sf::RenderTexture &renderTexture = GameWindow::instance().screen();

        auto bg_rect = this->selection_box(80, 560, has_switched);
        auto magic_sprite = this->magic_graphics[magic_index];
        magic_sprite->set_position({"center", bg_rect.center()});

        renderTexture.draw(magic_sprite->surf());
    }

    void show_exp(int exp)
    {
        sf::RenderTexture &renderTexture = GameWindow::instance().screen();

        this->text.setString(std::to_string(exp));
        auto size = GameWindow::instance().screen().getSize();
        size.x -= 20;
        size.y -= 20;

        auto text_rect = py::Rect<float>(text.getLocalBounds());
        text_rect.center({size.x - text_rect.width, size.y - text_rect.height});
        text.setPosition({text_rect.left + 10, text_rect.top + 5});

        auto box_rect = text_rect.inflate(20, 20);
        sf::RectangleShape box({box_rect.width, box_rect.height});
        box.setFillColor(UI_BG_COLOR);
        box.setPosition({box_rect.left, box_rect.top});

        sf::RectangleShape border({box_rect.width, box_rect.height});
        border.setOutlineThickness(3);
        border.setOutlineColor(UI_BORDER_COLOR);
        border.setFillColor(sf::Color::Transparent);
        border.setPosition({box_rect.left, box_rect.top});

        renderTexture.draw(box);
        renderTexture.draw(text);
        renderTexture.draw(border);
    }

    void show_bar(float current, float max_amount, const py::Rect<float> &bg_rect, const sf::Color &color)
    {
        sf::RenderTexture &renderTexture = GameWindow::instance().screen();

        sf::RectangleShape rectangle({bg_rect.width, bg_rect.height});
        rectangle.setFillColor(UI_BG_COLOR);
        rectangle.setPosition({bg_rect.left, bg_rect.top});

        auto ratio = current / max_amount;
        auto current_width = bg_rect.width * ratio;
        auto current_rect = py::Rect<float>(bg_rect);
        current_rect.width = current_width;

        sf::RectangleShape bar(sf::Vector2f(current_rect.width, current_rect.height));
        bar.setFillColor(color);
        bar.setPosition({bg_rect.left, bg_rect.top});

        sf::RectangleShape border(sf::Vector2f(bg_rect.width, bg_rect.height));
        border.setOutlineThickness(3);
        border.setOutlineColor(UI_BORDER_COLOR);
        border.setFillColor(sf::Color::Transparent);
        border.setPosition({bg_rect.left, bg_rect.top});

        renderTexture.draw(rectangle);
        renderTexture.draw(bar);
        renderTexture.draw(border);
    }

private:
    sf::Font font;
    sf::Text text;
    py::Rect<float> health_bar_rect;
    py::Rect<float> energy_bar_rect;
    std::vector<std::shared_ptr<SpriteTexture>> weapon_graphics;
    std::vector<std::shared_ptr<SpriteTexture>> magic_graphics;
};

#endif /* __UI_HPP__ */
