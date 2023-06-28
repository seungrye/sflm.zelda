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

class SpriteTexture {
public:
  SpriteTexture();
  SpriteTexture(unsigned width, unsigned height);
  SpriteTexture(const std::string &path);

  bool loadFromFile(const std::string &filename, const sf::IntRect &area = sf::IntRect());
 
  void move(const sf::Vector2f &offset);
  const sf::IntRect &getTextureRect();
  const py::Rect &get_rect(const std::pair<std::string, sf::Vector2u> &pos);
  const py::Rect &rect();
  const sf::Sprite &surf();

protected:
  std::shared_ptr<sf::Texture> texture_; //! 필수
  sf::Sprite sprite_;
  py::Rect rect_;
};

std::vector<std::vector<std::string>> import_csv_layout(const std::string &filename);
std::vector<std::shared_ptr<SpriteTexture>> import_folder(const std::string &folder);

#endif /* __SUPPORT_HPP__ */
