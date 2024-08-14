#include "upgrade.hpp"
#include "game_window.hpp"
#include "player.hpp"

Upgrade::Upgrade(std::shared_ptr<Player> player) : player(player) {
  const auto stats = player->stats();
  this->attribute_number = stats.size();
  for (const auto &item : stats) {
    this->attribute_names.push_back(item.first);
  }

  auto r = this->font.loadFromFile(UI_FONT);
  assert(r);

  this->text.setFont(this->font);
  this->text.setCharacterSize(UI_FONT_SIZE);

  const auto max_stats = player->max_stats();
  for (const auto &item : max_stats) {
    this->max_values.push_back(item.second);
  }

  sf::RenderTexture &renderTexture = GameWindow::instance().screen();
  this->height = renderTexture.getSize().y * 0.8;
  this->width = renderTexture.getSize().x / 6.f;

  this->create_items();

  this->selection_index = 0;
  this->can_move = true;
}

void Upgrade::create_items() {
  sf::RenderTexture &renderTexture = GameWindow::instance().screen();

  for (auto i = 0; i < this->attribute_names.size(); ++i) {
    // horizontal position
    auto full_width = renderTexture.getSize().x;
    auto increment = full_width / this->attribute_number;
    auto left = (increment * i) + ((increment - this->width) / 2);
    // vertical position
    auto top = renderTexture.getSize().y * 0.1;
    auto item = Item(left, top, this->width, this->height, i, this->text);
    this->item_list.push_back(item);
  }
}

void Upgrade::display() {
  this->input();
  this->selection_cooldown();

  auto index = 0;
  for (auto &item : this->item_list) {
    auto name = this->attribute_names[index];
    auto value = this->player->get_value_by_index(index);
    auto max_value = this->max_values[index];
    auto cost = this->player->get_cost_by_index(index);

    item.display(this->selection_index, name, value, max_value, cost);

    index++;
  }
}

void Upgrade::input() {
  if (this->can_move) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) &&
        this->selection_index < (this->attribute_number - 1)) {
      this->selection_index += 1;
      this->can_move = false;
      this->selection_time.restart();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) &&
               this->selection_index >= 1) {
      this->selection_index -= 1;
      this->can_move = false;
      this->selection_time.restart();
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      this->item_list[this->selection_index].trigger(this->player);
      this->can_move = false;
      this->selection_time.restart();
    }
  }
}

void Upgrade::selection_cooldown() {
  if (!this->can_move) {
    if (this->selection_time.getElapsedTime() > sf::milliseconds(300)) {
      this->can_move = true;
    }
  }
}

void Item::trigger(std::shared_ptr<Player> player) {
  auto stats = player->stats();
  auto upgrade_attribute = nth_name(stats, this->index);
  auto max_stats = player->max_stats();
  auto upgrade_cost = player->upgrade_cost();

  if (player->exp() > upgrade_cost[upgrade_attribute] &&
      stats[upgrade_attribute] < max_stats[upgrade_attribute]) {
    player->exp(player->exp() - upgrade_cost[upgrade_attribute]);
    player->stats(upgrade_attribute, stats[upgrade_attribute] * 1.2);
    player->upgrade_cost(upgrade_attribute,
                         upgrade_cost[upgrade_attribute] * 1.4);
  }

  stats = player->stats();
  if (stats[upgrade_attribute] > max_stats[upgrade_attribute]) {
    player->stats(upgrade_attribute, max_stats[upgrade_attribute]);
  }
}

void Item::display(int selection_num, const std::string &name, float value,
                   float max_value, int cost) {
  sf::RenderTexture &renderTexture = GameWindow::instance().screen();

  sf::RectangleShape rectangle({this->rect_.width, this->rect_.height});
  if (this->index == selection_num) {
    rectangle.setFillColor(UPGRADE_BG_COLOR_SELECTED);
  } else {
    rectangle.setFillColor(UI_BG_COLOR);
  }
  rectangle.setPosition({this->rect_.left, this->rect_.top});

  sf::RectangleShape border({this->rect_.width, this->rect_.height});
  border.setOutlineThickness(4);
  border.setFillColor(sf::Color::Transparent);
  border.setPosition({this->rect_.left, this->rect_.top});

  renderTexture.draw(rectangle);
  renderTexture.draw(border);

  this->display_name(name, cost, this->index == selection_num);
  this->display_bar(value, max_value, this->index == selection_num);
}

void Item::display_name(const std::string &name, int cost, bool selected) {
  sf::RenderTexture &renderTexture = GameWindow::instance().screen();
  auto color = (selected) ? TEXT_COLOR_SELECTED : TEXT_COLOR;

  // title
  this->text.setString(name);
  this->text.setFillColor(color);
  auto text_rect = py::Rect<float>(text.getLocalBounds());
  text_rect.set({"midtop", this->rect_.midtop() + sf::Vector2f(0, 20)});
  this->text.setPosition({text_rect.left, text_rect.top});
  renderTexture.draw(this->text);

  // cost
  this->text.setString(std::to_string(cost));
  this->text.setFillColor(color);
  auto cost_rect = py::Rect<float>(text.getLocalBounds());
  cost_rect.set({"midbottom", this->rect_.midbottom() - sf::Vector2f(0, 20)});
  this->text.setPosition({cost_rect.left, cost_rect.top});
  renderTexture.draw(this->text);
}

void Item::display_bar(float value, float max_value, bool selected) {
  sf::RenderTexture &renderTexture = GameWindow::instance().screen();
  auto color = (selected) ? BAR_COLOR_SELECTED : BAR_COLOR;

  auto top = this->rect_.midtop() + sf::Vector2f(0, 60);
  auto bottom = this->rect_.midbottom() - sf::Vector2f(0, 60);

  auto full_height = bottom.y - top.y;
  auto relative_number = (value / max_value) * full_height;
  auto value_rect =
      py::Rect<float>(top.x - 15, bottom.y - relative_number, 30, 10);

  sf::RectangleShape line({0, full_height});
  line.setOutlineColor(color);
  line.setOutlineThickness(2.5);
  line.setPosition(top);

  sf::RectangleShape rectangle({value_rect.width, value_rect.height});
  rectangle.setFillColor(color);
  rectangle.setPosition({value_rect.left, value_rect.top});

  renderTexture.draw(line);
  renderTexture.draw(rectangle);
}
