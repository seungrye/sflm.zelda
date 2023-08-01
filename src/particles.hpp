#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include "support.hpp"
#include "sprite_manager.hpp"

class ParticleEffect : public SpriteTexture
{
public:
    ParticleEffect(const py::Vector2f &pos, const std::vector<std::shared_ptr<SpriteTexture>> &sprite_frames, DeferredSpriteManager &sprite_manager);
    void animate();
    void update() override;

private:
    void update_sprite(std::shared_ptr<SpriteTexture> sprite);

    float frame_index;
    float animation_speed;
    std::vector<std::shared_ptr<SpriteTexture>> frames;
    DeferredSpriteManager &sprite_manager;
};

class AnimationPlayer
{
public:
    AnimationPlayer(DeferredSpriteManager &sprite_manager);
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
    std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> frames;
    std::vector<std::vector<std::shared_ptr<SpriteTexture>>> leaf_frames;
    DeferredSpriteManager &sprite_manager;
};

#endif /* __PARTICLES_HPP__ */
