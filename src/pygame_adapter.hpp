#ifndef __PYGAME_ADAPTER_HPP__
#define __PYGAME_ADAPTER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace py {
class Rect : public sf::IntRect {
public:
  Rect() {}
  Rect(sf::IntRect &rect) : sf::IntRect(rect) {
    centerx = left + (width / 2);
    centery = top + (height / 2);
  }
  Rect(int rectLeft, int rectTop, int rectWidth, int rectHeight)
      : sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight) {
    centerx = left + (width / 2);
    centery = top + (height / 2);
  }
  py::Rect inflate(int x, int y) {
    return py::Rect(this->left, this->top, this->width + x, this->height + y);
  }
  const sf::Vector2u center() { return {this->centerx, this->centery}; }

public:
  unsigned centerx;
  unsigned centery;
};
} // namespace py

#endif