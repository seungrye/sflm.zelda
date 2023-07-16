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
    Vector2(const sf::Vector2<T>& other) : sf::Vector2<T>(other) {}
    Vector2(T X, T Y) : sf::Vector2<T>(X, Y)
    {
    }

    /**
     * @brief calculates the Euclidean distance to a given vector
     * 
     * @param v 타겟 벡터
     * @return float Euclidean distance
     */
    float distance_to(const py::Vector2<T>& v) {
      auto x = v.x - this->x;
      auto y = v.y - this->y;
      return std::sqrt((x*x) + (y*y));
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
      return std::sqrt((this->x * this->x) + (this->y * this->y));
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

  template <typename T>
  class Rect : public sf::Rect<T>
  {
  public:
    Rect() {}
    Rect(const sf::Rect<T> &rect) : sf::Rect<T>(rect)
    {
      centerx = this->left + (this->width / 2);
      centery = this->top + (this->height / 2);
    }
    Rect(T rectLeft, T rectTop, T rectWidth, T rectHeight)
        : sf::Rect<T>(rectLeft, rectTop, rectWidth, rectHeight)
    {
      centerx = this->left + (this->width / 2);
      centery = this->top + (this->height / 2);
    }

    py::Rect<T> inflate(T x, T y)
    {
      return py::Rect<T>(this->left, this->top, this->width + x, this->height + y);
    }

    const sf::Vector2<T> center() { return {this->centerx, this->centery}; }

    /**
     * @brief 현재 객체의 center 를 파라메터로 전달된 위치로 변경
     *
     * @param center
     */
    void center(const sf::Vector2<T> &center)
    {
      this->centerx = center.x;
      this->centery = center.y;

      this->left = center.x - (this->width / 2);
      this->top = center.y - (this->height / 2);
    }

    void transform(T x, T y)
    {
      this->centerx += x;
      this->centery += y;

      this->left += x;
      this->top += y;
    }

  public:
    T centerx;
    T centery;
  };

  typedef Rect<int> IntRect;
  typedef Rect<float> FloatRect;

} // namespace py

#endif