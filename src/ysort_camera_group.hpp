#ifndef __YSORT_CAMERA_GROUP_HPP__
#define __YSORT_CAMERA_GROUP_HPP__

#include "player.hpp"
#include "support.hpp"
#include "pygame_adapter.hpp"
#include "sprite_manager.hpp"
#include <list>
#include <memory>

class YSortCameraGroup
{
public:
    YSortCameraGroup();
    void custom_draw(std::shared_ptr<Player> player);
    void update();
    void enemy_update(std::shared_ptr<Player> player);
    void push_back(std::shared_ptr<SpriteTexture> __x) { this->sprites.push_back(__x); }
    void remove(const std::shared_ptr<SpriteTexture> &__x) { this->sprites.remove(__x); }

    template <typename _Predicate>
    auto find_if(_Predicate __pred) -> std::list<std::shared_ptr<SpriteTexture>>::iterator
    {
        return std::find_if(this->sprites.begin(), this->sprites.end(), __pred);
    }

private:
    friend class SpriteManager;

    unsigned half_width;
    unsigned half_height;
    sf::Vector2f offset;

    SpriteTexture floor;
    std::list<std::shared_ptr<SpriteTexture>> sprites;
};

#endif /* __YSORT_CAMERA_GROUP_HPP__ */
