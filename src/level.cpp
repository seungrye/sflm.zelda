#include "level.hpp"
#include "settings.hpp"
#include "support.hpp"
#include "tile.hpp"
#include "enemy.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <iostream>

Level::Level() : game_paused(false)
{
  this->create_map();
}

void Level::create_map()
{
  std::map<std::string, std::vector<std::vector<std::string>>> layouts = {
      {"boundary", import_csv_layout("./src/map/map_FloorBlocks.csv")},
      {"grass", import_csv_layout("./src/map/map_Grass.csv")},
      {"object", import_csv_layout("./src/map/map_Objects.csv")},
      {"entities", import_csv_layout("./src/map/map_Entities.csv")},
  };

  std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> graphics = {
      {"grass", import_folder("./src/graphics/grass/")},
      {"object", import_folder("./src/graphics/objects/")}};

  for (const auto &entry : layouts)
  {
    auto style = entry.first;
    auto layout = entry.second;
    int row_index = 0;
    for (const auto &row : layout)
    {
      int col_index = 0;
      for (const auto &col : row)
      {
        if (!col.compare("-1"))
        {
          col_index++;
          continue;
        }

        auto x = col_index * TILESIZE;
        auto y = row_index * TILESIZE;

        if (!style.compare("boundary"))
        {
          auto tile = std::make_shared<Tile>(sf::Vector2f(x, y), "invisible");
          this->obstacle_sprites.push_back(tile);
        }
        else if (!style.compare("grass"))
        {
          auto grass = this->random_choice(graphics["grass"]);
          auto tile = std::make_shared<Tile>(sf::Vector2f(x, y), "grass", *grass);
          this->visible_sprites.push_back(tile);
          this->obstacle_sprites.push_back(tile);
        }
        else if (!style.compare("object"))
        {
          auto sprite = graphics["object"][std::stoi(col)];
          auto tile = std::make_shared<Tile>(sf::Vector2f(x, y), "object", *sprite);
          this->visible_sprites.push_back(tile);
          this->obstacle_sprites.push_back(tile);
          this->attackable_sprites.push_back(tile);
        }
        else if (!style.compare("entities"))
        {
          if (!col.compare("394"))
          {
            this->player = std::make_shared<Player>(sf::Vector2f(x, y), this->obstacle_sprites);
            this->visible_sprites.push_back(this->player);
          }
          else
          {
            auto monster_name = [&col]() -> std::string
            {
              if (!col.compare("390"))
                return "bamboo";
              if (!col.compare("391"))
                return "spirit";
              if (!col.compare("392"))
                return "raccoon";
              if (!col.compare("393"))
                return "squid";
              return "";
            }();
            if (!monster_name.empty())
            {
              auto enemy = std::make_shared<Enemy>(monster_name, sf::Vector2f(x, y), this->obstacle_sprites);
              this->visible_sprites.push_back(enemy);
              this->attackable_sprites.push_back(enemy);
            }
          }
        }

        col_index++;
      }
      row_index++;
    }
  }

#if false
  for (const auto& object: this->obstacle_sprites) {
    std::cout<<"(l,t) : ("<<object->hitbox().left<<",\t"<<object->hitbox().top<<")"
             <<"(r,b) : ("<<object->hitbox().left + object->hitbox().width<<",\t"<<object->hitbox().top+object->hitbox().height<<")"
             <<std::endl;
  }
#endif
}
void Level::create_attack() {}
void Level::destroy_attack() {}
void Level::run()
{
  visible_sprites.custom_draw(player);

  if (this->game_paused) {
    // this->upgrade.display();
  } else {
    visible_sprites.update();
    visible_sprites.enemy_update(this->player);
    // this->player_attack_logic();
  }
}

YSortCameraGroup::YSortCameraGroup()
{
  auto &rnderer = GameWindow::instance();
  this->half_width = rnderer.screen().getSize().x / 2;
  this->half_height = rnderer.screen().getSize().y / 2;
  this->offset = sf::Vector2f(0, 0);

  // creating the floor
  auto r = this->floor.loadFromFile("./src/graphics/tilemap/ground.png");
  assert(r);
}

void YSortCameraGroup::custom_draw(std::shared_ptr<Player> player)
{
  this->offset.x = player->rect().centerx - this->half_width;
  this->offset.y = player->rect().centery - this->half_height;

  // drawing the floor
  auto floor_offset_pos = sf::Vector2f(this->floor.rect().left - this->offset.x, this->floor.rect().top - this->offset.y);
  this->floor.move(floor_offset_pos);
  GameWindow::instance().screen().draw(this->floor.surf());

  std::sort(std::begin(this->sprites), std::end(this->sprites),
            [](auto a, auto b) -> bool
            {
              return a->rect().centery < b->rect().centery;
            });

  for (const auto &sprite : this->sprites)
  {
    auto offset_rect = sf::Vector2f(sprite->rect().left - this->offset.x, sprite->rect().top - this->offset.y);
    sprite->move(offset_rect);
    GameWindow::instance().screen().draw(sprite->surf());

    // draw hitbox (for debugging)
    do
    {
      sf::RectangleShape rectangle;
      rectangle.setSize(sf::Vector2f(sprite->rect().getSize()));
      rectangle.setOutlineColor(sf::Color::Blue);
      rectangle.setOutlineThickness(5);
      rectangle.setFillColor(sf::Color::Transparent);
      rectangle.setPosition(sf::Vector2f(sprite->rect().getPosition()));
      rectangle.setOrigin(-floor_offset_pos);

      sf::RectangleShape hitbox;
      hitbox.setSize(sf::Vector2f(sprite->hitbox().getSize()));
      hitbox.setOutlineColor(sf::Color::Red);
      hitbox.setOutlineThickness(3);
      hitbox.setFillColor(sf::Color::Transparent);
      hitbox.setPosition(sf::Vector2f(sprite->hitbox().getPosition()));
      hitbox.setOrigin(-floor_offset_pos);

      GameWindow::instance().screen().draw(rectangle);
      GameWindow::instance().screen().draw(hitbox);
    } while (false);
  }

  GameWindow::instance().screen().display();
}

void YSortCameraGroup::update() {
  for(const auto& sprite: this->sprites) {
    sprite->update();
  }
}
