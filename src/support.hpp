#ifndef __SUPPORT_HPP__
#define __SUPPORT_HPP__

#include "pygame_adapter.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <memory>
#include <string>
#include <vector>

class SpriteTexture
{
public:
  SpriteTexture();
  SpriteTexture(unsigned width, unsigned height);
  SpriteTexture(const std::string &path);
  SpriteTexture(const sf::Texture &texture);

  bool loadFromFile(const std::string &filename, const sf::IntRect &area = sf::IntRect());

  void set_origin(const sf::Vector2f &offset);
  void set_position(const std::pair<std::string, sf::Vector2f> &pos)
  {
    if (!pos.first.compare("center"))
    {
      this->get_rect(pos);
    }
    this->sprite_.setPosition({this->rect_.left, this->rect_.top});
  }
  const py::Rect<float> &get_rect(const std::pair<std::string, sf::Vector2f> &pos);
  const py::Rect<float> &rect();
  const sf::Sprite &surf();
  const py::Rect<float> &hitbox();
  const py::Rect<float> get_global_bounds()
  {
    return py::Rect<float>(this->sprite_.getGlobalBounds());
  }
  const std::string &sprite_type() { return this->sprite_type_; }

  /**
   * @brief sprite 의 hitbox 와 충될되는 면적이 있는지 여부를 반환
   *
   * @param rect 대상 면적
   * @return true
   * @return false
   */
  bool colliderect(const py::Rect<float> &rect, sf::Rect<float> &intersection)
  {
    return this->hitbox_.intersects(rect, intersection);
  }

  virtual void update() {}

  /**
   * @brief 특정 타입의 sprite 인지 체크
   *
   * @param sprite_type
   * @return true 파라메터로 전달된 타입과 매칭될 경우
   * @return false 파라메터로 전달된 타입과 매칭되지 않을 경우
   */
  bool is(std::string sprite_type)
  {
    return !this->sprite_type_.compare(sprite_type);
  }

  void flip(bool x, bool y)
  {

    this->sprite_.setOrigin({this->sprite_.getLocalBounds().width, 0});
    this->sprite_.setScale({x ? -1.f : 1.f, y ? -1.f : 1.f});
  }

private:
  const sf::IntRect &getTextureRect();

public:
  std::shared_ptr<sf::Texture> texture_; //! 필수

protected:
  sf::Sprite sprite_;
  py::Rect<float> rect_;
  py::Rect<float> hitbox_;
  std::string sprite_type_;
};

std::vector<std::vector<std::string>> import_csv_layout(const std::string &filename);
std::vector<std::shared_ptr<SpriteTexture>> import_folder(const std::string &folder);

#endif /* __SUPPORT_HPP__ */
