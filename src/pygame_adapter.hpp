#ifndef __PYGAME_ADAPTER_HPP__
#define __PYGAME_ADAPTER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>

namespace py
{
  class Rect : public sf::IntRect
  {
  public:
    Rect() {}
    Rect(sf::IntRect &rect) : sf::IntRect(rect)
    {
      centerx = left + (width / 2);
      centery = top + (height / 2);
    }
    Rect(int rectLeft, int rectTop, int rectWidth, int rectHeight)
        : sf::IntRect(rectLeft, rectTop, rectWidth, rectHeight)
    {
      centerx = left + (width / 2);
      centery = top + (height / 2);
    }
    py::Rect inflate(int x, int y)
    {
      return py::Rect(this->left, this->top, this->width + x, this->height + y);
    }
    const sf::Vector2u center() { return {this->centerx, this->centery}; }

    /**
     * @brief 현재 객체의 center 를 파라메터로 전달된 위치로 변경
     *
     * @param center
     */
    void center(const sf::Vector2u &center)
    {
      this->centerx = center.x;
      this->centery = center.y;

      this->left = center.x - (width / 2);
      this->top = center.y - (height / 2);
    }

    void transform(int x, int y)
    {
      this->centerx += x;
      this->centery += y;

      this->left += x;
      this->top += y;
    }

  public:
    unsigned centerx;
    unsigned centery;
  };
} // namespace py

#endif