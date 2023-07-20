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
           const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites)
        : frame_index(frame_index), animation_speed(animation_speed), direction(direction), obstacle_sprites(obstacle_sprites) {}

    int wave_value()
    {
        // see: https://stackoverflow.com/a/69729769
        static auto n = 0;
        if (n >= 2500)
            n = 0;
        auto value = sin(2 * M_PI * 8.4e-3 * n);
        if (value > 0)
            return 255;
        else
            return 0;
    }

    void move(float speed)
    {
        // move same speed in x and y direction
        if (this->direction.magnitude() != 0)
        {
            this->direction = this->direction.normalize();
        }

        this->hitbox_.transform(this->direction.x * speed, 0);
        this->collision("horizontal");
        this->hitbox_.transform(0, this->direction.y * speed);
        this->collision("vertical");

        this->rect_.center(this->hitbox_.center());
    }

private:
    void collision(const std::string &direction)
    {
        sf::Rect<float> intersection;
        if (!direction.compare("horizontal"))
        {
            for (const auto &sprite : this->obstacle_sprites)
            {
                if (sprite->colliderect(this->hitbox_, intersection))
                {
                    if (this->direction.x > 0)
                    { // move right
                        this->hitbox_.transform(intersection.width * -1, 0);
                    }
                    else if (this->direction.x < 0)
                    { // move left
                        this->hitbox_.transform(intersection.width, 0);
                    }
                }
            }
        }
        else if (!direction.compare("vertical"))
        {
            for (const auto &sprite : this->obstacle_sprites)
            {
                if (sprite->colliderect(this->hitbox_, intersection))
                {
                    if (this->direction.y > 0)
                    { // move down
                        this->hitbox_.transform(0, intersection.height * -1);
                    }
                    else if (this->direction.y < 0)
                    { // move up
                        this->hitbox_.transform(0, intersection.height);
                    }
                }
            }
        }
    }

protected:
    float frame_index;
    float animation_speed;
    py::Vector2f direction;
    const std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites;
};

#endif /* __ENTITY_HPP__ */
