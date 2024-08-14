#include "level.hpp"
#include "settings.hpp"
#include "support.hpp"
#include "tile.hpp"
#include "enemy.hpp"
#include "weapon.hpp"
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

Level::Level()
    : game_paused(false),
      animation_player(std::make_shared<AnimationPlayer>(*this)),
      magic_player(std::make_shared<MagicPlayer>(this->animation_player))
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
          auto grass = this->random_choice<std::vector<std::shared_ptr<SpriteTexture>>>(graphics["grass"]);
          auto tile = std::make_shared<Tile>(sf::Vector2f(x, y), "grass", *grass);
          this->visible_sprites.push_back(tile);
          this->obstacle_sprites.push_back(tile);
          this->attackable_sprites.push_back(tile);
        }
        else if (!style.compare("object"))
        {
          auto sprite = graphics["object"][std::stoi(col)];
          auto tile = std::make_shared<Tile>(sf::Vector2f(x, y), "object", *sprite);
          this->visible_sprites.push_back(tile);
          this->obstacle_sprites.push_back(tile);
        }
        else if (!style.compare("entities"))
        {
          if (!col.compare("394"))
          {
            this->player = std::make_shared<Player>(
                sf::Vector2f(x, y),
                this->obstacle_sprites,
                [this]() -> void
                { this->create_attack(); },
                [this]() -> void
                {
                  this->destroy_attack();
                },
                [this](const std::string &style, int strength, int cost)
                {
                  this->create_magic(style, strength, cost);
                },
                [this]() -> void
                {
                  this->destroy_magic();
                },
                [this](int amount, const std::string &attack_type) -> void
                { this->damage_player(amount, attack_type); });
            this->upgrade = std::make_shared<Upgrade>(this->player);
            this->visible_sprites.push_back(this->player);
          }
        }

        col_index++;
      }
      row_index++;
    }
  }

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

        if (!style.compare("entities"))
        {
          if (col.compare("394")) // not 394
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
              assert(false);
            }();
            if (!monster_name.empty())
            {
              auto enemy = std::make_shared<Enemy>(
                  monster_name,
                  sf::Vector2f(x, y),
                  this->obstacle_sprites,
                  [this](const sf::Vector2f &pos, const std::string &monster_name)
                  {
                    this->trigger_death_particles(pos, monster_name);
                  },
                  [this](int exp)
                  {
                    this->add_exp(exp);
                  },
                  *this,
                  this->player);
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
}

void Level::trigger_death_particles(const sf::Vector2f &pos, const std::string &particle_type)
{
  auto sprite = this->animation_player->create_particles(pos, particle_type);
  this->visible_sprites.push_back(sprite);
}

void Level::add_exp(int amount)
{
  this->player->exp(this->player->exp() + amount);
}

void Level::create_attack()
{
  this->current_attack = std::make_shared<Weapon>(this->player);
  this->visible_sprites.push_back(this->current_attack);
  this->attack_sprites.push_back(this->current_attack);
}

void Level::destroy_attack()
{
  if (this->current_attack)
  {
    this->kill(this->current_attack);
    this->current_attack = nullptr;
  }
}

void Level::create_magic(const std::string &style, int strength, int cost)
{
  if (!style.compare("heal"))
  {
    auto sprites = this->magic_player->heal(this->player, strength, cost);
    for (const auto &sprite : sprites)
    {
      this->visible_sprites.push_back(sprite);
    }
  }
  else if (!style.compare("flame"))
  {
    auto sprites = this->magic_player->flame(this->player, cost);
    for (const auto &sprite : sprites)
    {
      this->visible_sprites.push_back(sprite);
      this->attack_sprites.push_back(sprite);
    }
  }
}

void Level::destroy_magic()
{
}

void Level::run()
{
  this->visible_sprites.custom_draw(player);
  this->ui.display(this->player);

  if (this->game_paused)
  {
    this->upgrade->display();
  }
  else
  {
    this->visible_sprites.update();
    this->player_attack_logic();
    this->remove_dead_sprites();
  }
}

void Level::player_attack_logic()
{
  for (const auto &attack_sprite : this->attack_sprites)
  {
    auto collision_sprites = ::spritecollide(attack_sprite, this->attackable_sprites);
    for (const auto &collision_sprite : collision_sprites)
    {
      if (!collision_sprite->sprite_type().compare("grass"))
      {
        auto rect = collision_sprite->rect();
        auto pos = rect.center();
        auto offset = py::Vector2f(0, 75);
        int range = 3 + (rand() % 4); // 3 ~ 6
        for (auto i = 0; i < range; ++i)
        {
          auto particle = this->animation_player->create_grass_particles(pos - offset);
          this->visible_sprites.push_back(particle);
        }

        this->kill(collision_sprite);
      }
      else if (!collision_sprite->sprite_type().compare("enemy"))
      {
        auto enemy = std::static_pointer_cast<Enemy>(collision_sprite);
        enemy->get_damage(this->player, attack_sprite->sprite_type());
      }
    }
  }
}

void Level::damage_player(int amount, const std::string &attack_type)
{
  if (this->player->vulernable())
  {
    this->player->health(this->player->health() - amount);
    this->player->vulernable(false);
    this->player->restart_hurt_time();
    auto rect = this->player->rect();
    auto pos = rect.center();
    auto sprite = this->animation_player->create_particles(pos, attack_type);
    this->visible_sprites.push_back(sprite);
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
  this->floor.set_origin(floor_offset_pos);
  GameWindow::instance().screen().draw(this->floor.surf());

  this->sprites.sort(
      [](auto a, auto b) -> bool
      {
        return a->rect().centery < b->rect().centery;
      });

  for (const auto &sprite : this->sprites)
  {
    auto offset_rect = sf::Vector2f(sprite->rect().left - this->offset.x, sprite->rect().top - this->offset.y);
    sprite->set_origin(offset_rect);
    GameWindow::instance().screen().draw(sprite->surf());

#if 0 // draw hitbox (for debugging)
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
#endif
  }

  GameWindow::instance().screen().display();
}

void YSortCameraGroup::update()
{
  for (const auto &sprite : this->sprites)
  {
    sprite->update();
  }
}
