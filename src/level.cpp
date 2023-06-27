#include "level.hpp"
#include "settings.hpp"
#include "support.hpp"
#include "tile.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <iterator>
#include <map>
#include <string>
#include <utility>
#include <vector>

Level::Level() { this->create_map(); }
void Level::create_map() {
  std::map<std::string, std::vector<std::vector<std::string>>> layouts = {
      {"boundary", import_csv_layout("../src/map/map_FloorBlocks.csv")},
      {"grass", import_csv_layout("../src/map/map_Grass.csv")},
      {"object", import_csv_layout("../src/map/map_Objects.csv")}};

  std::map<std::string, std::vector<SpriteTexture>> graphics = {
      {"grass", import_folder("../src/graphics/grass/")},
      {"object", import_folder("../src/graphics/objects/")}};

  for (const auto &entry : layouts) {
    auto style = entry.first;
    auto layout = entry.second;
    int row_index = 0;
    for (const auto &row : layout) {
      int col_index = 0;
      for (const auto &col : row) {
        if (col.compare("-1"))
          continue;
        auto x = col_index * TILESIZE;
        auto y = row_index * TILESIZE;

        if (style.compare("boundary")) {
          auto sprite_groups = std::vector<decltype(this->obstacle_sprites)>{
              this->obstacle_sprites};
          Tile(sf::Vector2u(x, y), sprite_groups, "invisible");
        } else if (style.compare("grass")) {
          auto limit = graphics["grass"].size();
          auto grass = graphics["grass"][random() % limit];
          auto sprite_groups = std::vector<decltype(this->obstacle_sprites)>{
              this->visible_sprites, this->obstacle_sprites};
          Tile(sf::Vector2u(x, y), sprite_groups, "grass", grass);
        } else if (style.compare("object")) {
          auto sprite = graphics["object"][std::stoi(col)];
          auto sprite_groups = std::vector<decltype(this->obstacle_sprites)>{
              this->visible_sprites, this->obstacle_sprites};
          Tile(sf::Vector2u(x, y), sprite_groups, "object", sprite);
        }

        col_index++;
      }
      row_index++;
    }
  }

  this->player = Player(
      sf::Vector2u(1500, 1500),
      std::vector<decltype(this->obstacle_sprites)>{this->visible_sprites},
      this->obstacle_sprites);
}
void Level::create_attack() {}
void Level::destroy_attack() {}
void Level::run() {
  visible_sprites.custom_draw(player);
  visible_sprites.update();
}

YSortCameraGroup::YSortCameraGroup() {
  auto &rnderer = GameWindow::instance();
  this->half_width = rnderer.screen().getSize().x / 2;
  this->half_height = rnderer.screen().getSize().y / 2;
  this->offset = sf::Vector2u(0, 0);

  // creating the floor
  auto r = this->floor.loadFromFile("../src/graphics/tilemap/ground.png");
  assert(r);
}

void YSortCameraGroup::custom_draw(Player &player) {
  this->offset.x = player.rect.centerx - this->half_width;
  this->offset.y = player.rect.centery - this->half_height;

  // drawing the floor
  auto floor_offset_pos = sf::Vector2f(this->floor.rect().left - this->offset.x,
                                       this->floor.rect().top - this->offset.y);

  auto &renderer = GameWindow::instance();
  // renderer.screen().clear();

  this->floor.move(floor_offset_pos);
  renderer.screen().draw(this->floor.surf());

  std::sort(std::begin(this->sprites), std::end(this->sprites),
            [](auto a, auto b) -> bool {
              return a.rect().centery - b.rect().centery;
            });

  for (auto &sprite : this->sprites) {
    auto offset_rect = sf::Vector2f(sprite.rect().left - this->offset.x,
                                    sprite.rect().top - this->offset.y);
    sprite.move(offset_rect);
    renderer.screen().draw(sprite.surf());
  }

  renderer.screen().display();
}

void YSortCameraGroup::update() {}
