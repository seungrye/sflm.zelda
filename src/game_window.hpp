#ifndef __GAME_WINDOW_HPP__
#define __GAME_WINDOW_HPP__

#include "settings.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Texture.hpp>

class GameWindow {
public:
  static GameWindow &instance() {
    static GameWindow instance;
    return instance;
  }
  sf::RenderTexture &screen() { return this->render_texture; }

private:
  GameWindow() { this->render_texture.create(WIDTH, HEIGHT); }

  ~GameWindow() { /*clean-up*/
  }

  // 복사 생성자와 대입 연산자를 비활성화
  GameWindow(const GameWindow &) = delete;
  GameWindow &operator=(const GameWindow &) = delete;

private:
  sf::RenderTexture render_texture;
};

#endif