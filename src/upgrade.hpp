#ifndef __UPGRADE_HPP__
#define __UPGRADE_HPP__

#include "settings.hpp"
#include "pygame_adapter.hpp"
#include <memory>
#include <vector>

class Player;

class Item
{
public:
    Item(float l, float t, float w, float h, int index, const sf::Text &text)
        : rect_(l, t, w, h), index(index), text(text) {}
    void trigger(std::shared_ptr<Player> player);
    void display(int selection_num, const std::string &name, int value, int max_value, int cost);

private:
    const py::Rect<float> rect_;
    const int index;
    const sf::Text &text;
};

class Upgrade
{
public:
    Upgrade(std::shared_ptr<Player> player);
    void display();

private:
    void create_items();
    void input();
    void selection_cooldown();

private:
    std::shared_ptr<Player> player;
    int attribute_number;
    std::vector<std::string> attribute_names;
    std::vector<Item> item_list;
    sf::Font font;
    sf::Text text;
    std::vector<int> max_values;
    float height;
    float width;
    int selection_index;
    sf::Clock selection_time;
    bool can_move;
};

#endif /* __UPGRADE_HPP__ */
