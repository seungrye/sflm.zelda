#ifndef __PYGAME_ADAPTER_HPP__
#define __PYGAME_ADAPTER_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace py
{
  template <typename T>
  class Vector2 : public sf::Vector2<T>
  {
  public:
    Vector2() = default;
    Vector2(T X, T Y) : sf::Vector2<T>(X, Y)
    {
    }
    /**
     * @brief 벡터의 유클리드 크기 반환
     * @see http://www.fundza.com/vectors/normalize/
     * @see https://runebook.dev/ko/docs/pygame/ref/math
     *
     * @return float
     */
    float magnitude()
    {
      return sqrt((this->x * this->x) + (this->y * this->y));
    }

    /**
     * @brief 방향은 같지만, 길이가 1인 벡터 반환
     * @see https://runebook.dev/ko/docs/pygame/ref/math
     * @see https://eastroot1590.tistory.com/entry/%EB%8B%A8%EC%9C%84%EB%B2%A1%ED%84%B0-%EA%B5%AC%ED%95%98%EB%8A%94-%EA%B3%B5%EC%8B%9Dvector-normalize
     *
     * @return py::Vector2<T>
     */
    py::Vector2<T> normalize()
    {
      auto length = this->magnitude();
      return {static_cast<T>(this->x / length), static_cast<T>(this->y / length)};
    }
  };

  typedef Vector2<int> Vector2i;
  typedef Vector2<unsigned int> Vector2u;
  typedef Vector2<float> Vector2f;

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