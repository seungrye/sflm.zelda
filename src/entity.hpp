#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include "support.hpp"
#include <list>

class Entity : public SpriteTexture
{
public:
    Entity(const float &frame_index,
           const float &animation_speed,
           const py::Vector2f &direction,
           const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites);
    int wave_value();
    void move(float speed);

private:
    void collision(const std::string &direction);

protected:
    float frame_index;
    float animation_speed;
    py::Vector2f direction;
    const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites;
};

#endif /* __ENTITY_HPP__ */
