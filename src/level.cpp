#include "level.hpp"
#include "support.hpp"
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>
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

  // TODO:
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

  for (auto i = std::begin(this->sprites); i != std::end(this->sprites); ++i) {
    auto offset_rect = sf::Vector2f(i->rect().left - this->offset.x,
                                    i->rect().top - this->offset.y);
    i->move(offset_rect);
    renderer.screen().draw(i->surf());
  }

  renderer.screen().display();
}

void YSortCameraGroup::update() {}
