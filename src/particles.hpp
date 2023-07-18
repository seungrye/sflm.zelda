#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include "support.hpp"

// TODO:
class ParticleEffect : public SpriteTexture
{
public:
    ParticleEffect(const sf::Vector2f &pos, const std::vector<std::shared_ptr<SpriteTexture>> &sprites)
    {
    }

private:
};

class AnimationPlayer
{
public:
    AnimationPlayer()
        : frames(
              {// magic
               {"flame", import_folder("./src/graphics/particles/flame/frames")},
               {"aura", import_folder("./src/graphics/particles/aura")},
               {"heal", import_folder("./src/graphics/particles/heal/frames")},

               // attack
               {"claw", import_folder("./src/graphics/particles/claw")},
               {"slash", import_folder("./src/graphics/particles/slash")},
               {"sparkle", import_folder("./src/graphics/particles/sparkle")},
               {"leaf_attack", import_folder("./src/graphics/particles/leaf_attack")},
               {"thunder", import_folder("./src/graphics/particles/thunder")},

               // monster deaths
               {"squid", import_folder("./src/graphics/particles/smoke_orange")},
               {"raccoon", import_folder("./src/graphics/particles/raccoon")},
               {"spirit", import_folder("./src/graphics/particles/nova")},
               {"bamboo", import_folder("./src/graphics/particles/bamboo")}}),
          leaf_frames(
              {import_folder("./src/graphics/particles/leaf1"),
               import_folder("./src/graphics/particles/leaf2"),
               import_folder("./src/graphics/particles/leaf3"),
               import_folder("./src/graphics/particles/leaf4"),
               import_folder("./src/graphics/particles/leaf5"),
               import_folder("./src/graphics/particles/leaf6")})
    {
        auto end = leaf_frames.end();
        for (auto item = leaf_frames.begin(); item != end; ++item)
        {
            leaf_frames.push_back(this->reflect_images(*item));
        }
    }

    std::shared_ptr<SpriteTexture> create_grass_particles(const py::Vector2f &pos)
    {
        auto animation_frames = this->random_choice<std::vector<std::shared_ptr<SpriteTexture>>>(this->leaf_frames);
        return std::make_shared<ParticleEffect>(pos, animation_frames);
    }

    std::shared_ptr<SpriteTexture> create_particles(const py::Vector2f &pos, const std::string &attack_type)
    {
        auto animation_frames = this->frames[attack_type];
        return std::make_shared<ParticleEffect>(pos, animation_frames);
    }

private:
    template <typename T>
    const T &random_choice(const std::vector<T> &list)
    {
        return list[random() % list.size()];
    }

    std::vector<std::shared_ptr<SpriteTexture>> reflect_images(const std::vector<std::shared_ptr<SpriteTexture>> &frames)
    {
        auto new_frames = std::vector<std::shared_ptr<SpriteTexture>>(frames.size());
        for (const auto &frame : frames)
        {
            auto sprite = std::make_shared<SpriteTexture>(*(frame->texture_));
            sprite->flip(true, false);
            new_frames.push_back(sprite);
        }
        return new_frames;
    }

private:
    std::map<std::string, std::vector<std::shared_ptr<SpriteTexture>>> frames;
    std::vector<std::vector<std::shared_ptr<SpriteTexture>>> leaf_frames;
};

#endif /* __PARTICLES_HPP__ */
