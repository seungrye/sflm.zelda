#include "game_window.hpp"
#include "level.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cassert>
#include <climits>
#include <unistd.h>

class Game {
public:
  Game() {}

  void run() {
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "sfmlZelda");
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(FPS);

    while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
          return window.close();
        }
        if (event.type == sf::Event::KeyReleased) {
          if (event.key.code == sf::Keyboard::M) {
            this->level.toggle_menu();
          }
        }
      }

      // 게임 로직을 여기에서 업데이트합니다.
      level.run();

      // (off-screen render 된 texture 를) 화면에 출력 합니다.
      window.clear();
      const sf::Texture &texture = GameWindow::instance().screen().getTexture();
      sf::Sprite sprite(texture);
      window.draw(sprite);
      window.display();
    }
  }

private:
  Level level;
};

void print_cwd() {
  char cwd[PATH_MAX];
  assert(getcwd(cwd, sizeof(cwd)) != NULL);
  printf("Current working dir: %s\n", cwd);
}

int main(int argc, char *argv[]) {
  print_cwd();

  Game game;
  game.run();

  return 0;
}
