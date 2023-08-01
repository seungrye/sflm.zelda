#include "support.hpp"
#include "level.hpp"
#include "pygame_adapter.hpp"
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

SpriteTexture::SpriteTexture() : texture_(std::make_shared<sf::Texture>()) {}
SpriteTexture::SpriteTexture(const sf::Texture &texture) : texture_(std::make_shared<sf::Texture>(texture))
{
  this->sprite_.setTexture(*this->texture_);
  auto rect = this->sprite_.getTextureRect();
  this->rect_ = py::Rect<float>(rect.left, rect.top, rect.width, rect.height);
}

SpriteTexture::SpriteTexture(unsigned width, unsigned height)
    : texture_(std::make_shared<sf::Texture>())
{
  auto r = this->texture_->create(width, height);
  assert(r);
  this->sprite_.setTexture(*this->texture_);
  auto rect = this->sprite_.getTextureRect();
  this->rect_ = py::Rect<float>(rect.left, rect.top, rect.width, rect.height);
}

SpriteTexture::SpriteTexture(const std::string &path)
    : texture_(std::make_shared<sf::Texture>())
{
  auto r = this->loadFromFile(path);
  assert(r);
}

bool SpriteTexture::loadFromFile(const std::string &filename,
                                 const sf::IntRect &area)
{
  auto r = this->texture_->loadFromFile(filename);
  if (r)
  {
    this->sprite_.setTexture(*this->texture_);
    auto rect = this->sprite_.getTextureRect();
    this->rect_ = py::Rect<float>(rect.left, rect.top, rect.width, rect.height);
  }
  return r;
}

const sf::IntRect &SpriteTexture::getTextureRect()
{
  return this->sprite_.getTextureRect();
}

void SpriteTexture::set_origin(const sf::Vector2f &offset)
{
  this->sprite_.setOrigin(-offset);
}

const sf::Sprite &SpriteTexture::surf() { return this->sprite_; }

const py::Rect<float> &
SpriteTexture::get_rect(const std::pair<std::string, sf::Vector2f> &pos)
{
  if (!pos.first.compare("topleft"))
  {
    this->rect_ = py::Rect<float>(pos.second.x, pos.second.y,
                                  this->rect_.width, this->rect_.height);
  }
  else if (!pos.first.compare("center"))
  {
    this->rect_.center(pos.second);
  }
  else if (!pos.first.compare("midleft"))
  {
    this->rect_ = py::Rect<float>(pos.second.x, pos.second.y - (this->rect_.height / 2),
                                  this->rect_.width, this->rect_.height);
  }
  else if (!pos.first.compare("midright"))
  {
    this->rect_ = py::Rect<float>(pos.second.x - this->rect_.width, pos.second.y - (this->rect_.height / 2),
                                  this->rect_.width, this->rect_.height);
  }
  else if (!pos.first.compare("midbottom"))
  {
    this->rect_ = py::Rect<float>(pos.second.x - (this->rect_.width / 2), pos.second.y - (this->rect_.height),
                                  this->rect_.width, this->rect_.height);
  }
  else if (!pos.first.compare("midtop"))
  {
    this->rect_ = py::Rect<float>(pos.second.x - (this->rect_.width / 2), pos.second.y,
                                  this->rect_.width, this->rect_.height);
  }
  return this->rect_;
}

auto SpriteTexture::set_position(const std::pair<std::string, sf::Vector2f> &pos) -> void
{
  if (!pos.first.compare("center"))
  {
    this->get_rect(pos);
  }
  this->sprite_.setPosition({this->rect_.left, this->rect_.top});
}

auto SpriteTexture::get_global_bounds() -> const py::Rect<float>
{
  return py::Rect<float>(this->sprite_.getGlobalBounds());
}

/**
 * @brief sprite 의 hitbox 와 충될되는 면적이 있는지 여부를 반환
 *
 * @param rect 대상 면적
 * @return true
 * @return false
 */
bool SpriteTexture::colliderect(const py::Rect<float> &rect, sf::Rect<float> &intersection)
{
  return this->hitbox_.intersects(rect, intersection);
}

/**
 * @brief 특정 타입의 sprite 인지 체크
 *
 * @param sprite_type
 * @return true 파라메터로 전달된 타입과 매칭될 경우
 * @return false 파라메터로 전달된 타입과 매칭되지 않을 경우
 */
bool SpriteTexture::is(std::string sprite_type)
{
  return !this->sprite_type_.compare(sprite_type);
}

void SpriteTexture::flip(bool x, bool y)
{

  this->sprite_.setOrigin({this->sprite_.getLocalBounds().width, 0});
  this->sprite_.setScale({x ? -1.f : 1.f, y ? -1.f : 1.f});
}

const py::Rect<float> &SpriteTexture::rect() { return this->rect_; }

const py::Rect<float> &SpriteTexture::hitbox() { return this->hitbox_; }

std::vector<std::vector<std::string>>
import_csv_layout(const std::string &filename)
{
  auto terrain_map = std::vector<std::vector<std::string>>();

  std::ifstream file(filename);
  std::string line;
  while (std::getline(file, line))
  {
    std::vector<std::string> row;
    std::stringstream line_stream(line);
    std::string cell;
    while (std::getline(line_stream, cell, ','))
    {
      row.push_back(cell);
    }
    terrain_map.push_back(row);
  }

  return std::move(terrain_map);
}

std::vector<std::shared_ptr<SpriteTexture>>
import_folder(const std::string &folder)
{
  auto surface_list = std::vector<std::shared_ptr<SpriteTexture>>{};

  for (const auto &entry :
       std::experimental::filesystem::directory_iterator(folder))
  {
    const auto &path = entry.path();

    if (std::experimental::filesystem::is_regular_file(entry))
    {
      surface_list.push_back(std::make_shared<SpriteTexture>(path));
    }
  }

  return surface_list;
}