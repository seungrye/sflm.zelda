#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include "support.hpp"
#include "sprite_manager.hpp"

class ParticleEffect : public SpriteTexture
{
public:
    ParticleEffect(const py::Vector2f &pos, const std::string& sprite_type, SpriteManager &sprite_manager);
    void animate();
    void update() override;

private:
    void update_sprite(const sf::Texture& texture);

    const py::Vector2f pos;
    float frame_index;
    float animation_speed;
    const std::vector<sf::Texture>& frames;
    SpriteManager &sprite_manager;
};

class AnimationPlayer
{
public:
    AnimationPlayer(SpriteManager &sprite_manager);
    std::shared_ptr<SpriteTexture> create_grass_particles(const py::Vector2f &pos);
    std::shared_ptr<SpriteTexture> create_particles(const py::Vector2f &pos, const std::string &attack_type);

private:
    template <typename T>
    const T &random_choice(const std::vector<T> &list)
    {
        return list[random() % list.size()];
    }

    std::vector<std::shared_ptr<SpriteTexture>> reflect_images(const std::vector<std::shared_ptr<SpriteTexture>> &frames);

private:
    SpriteManager &sprite_manager;
};

#endif /* __PARTICLES_HPP__ */
