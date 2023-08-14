#include "particles.hpp"

ParticleEffect::ParticleEffect(const py::Vector2f &pos, const std::string& sprite_type, SpriteManager &sprite_manager)
    : sprite_manager(sprite_manager),
    frames(sprite_manager.textures(sprite_type)),
    pos(pos)
{
    this->sprite_type_ = "magic";
    this->frame_index = 0;
    this->animation_speed = 0.15f;

    this->update_sprite(this->frames[static_cast<int>(this->frame_index)]);
}

void ParticleEffect::animate()
{
    this->frame_index += this->animation_speed;
    if (this->frame_index >= static_cast<float>(this->frames.size()))
    {
        this->frame_index = 0.f;
        this->sprite_manager.kill(this);
    }
    else
    {
        this->update_sprite(this->frames[static_cast<int>(this->frame_index)]);
    }
}

void ParticleEffect::update()
{
    this->animate();
}

void ParticleEffect::update_sprite(const sf::Texture& texture)
{
    this->sprite_.setTexture(texture);
    // set position
    auto rect = this->sprite_.getTextureRect();
    this->rect_ = py::Rect<float>(rect.left, rect.top, rect.width, rect.height);
    this->get_rect({"center", this->pos});
}

AnimationPlayer::AnimationPlayer(SpriteManager &sprite_manager)
    : sprite_manager(sprite_manager)
{
    sprite_manager.import("flame", "./src/graphics/particles/flame/frames");
    sprite_manager.import("aura", "./src/graphics/particles/aura");
    sprite_manager.import("heal", "./src/graphics/particles/heal/frames");
    sprite_manager.import("claw", "./src/graphics/particles/claw");
    sprite_manager.import("slash", "./src/graphics/particles/slash");
    sprite_manager.import("sparkle", "./src/graphics/particles/sparkle");
    sprite_manager.import("leaf_attack", "./src/graphics/particles/leaf_attack");
    sprite_manager.import("thunder", "./src/graphics/particles/thunder");
    sprite_manager.import("squid", "./src/graphics/particles/smoke_orange");
    sprite_manager.import("raccoon", "./src/graphics/particles/raccoon");
    sprite_manager.import("spirit", "./src/graphics/particles/nova");
    sprite_manager.import("bamboo", "./src/graphics/particles/bamboo");
    sprite_manager.import("leaf1", "./src/graphics/particles/leaf1");
    sprite_manager.import("leaf2", "./src/graphics/particles/leaf2");
    sprite_manager.import("leaf3", "./src/graphics/particles/leaf3");
    sprite_manager.import("leaf4", "./src/graphics/particles/leaf4");
    sprite_manager.import("leaf5", "./src/graphics/particles/leaf5");
    sprite_manager.import("leaf6", "./src/graphics/particles/leaf6");

    // TBD: leaf texture 들이 왼쪽 이미지 밖에 없어서, x 축으로 flip 한 texture 를 만들어서
    // 양쪽으로 흩날리도록 하기 위한 코드. (위 변경된 코드에 맞게 수정 필요)
    // decltype(leaf_frames) reflected_leaf_frames;
    // for (auto item = leaf_frames.begin(); item != leaf_frames.end(); ++item)
    // {
    //     reflected_leaf_frames.push_back(this->reflect_images(*item));
    // }
    // this->leaf_frames.insert(
    //     this->leaf_frames.end(),
    //     reflected_leaf_frames.begin(),
    //     reflected_leaf_frames.end());
}

std::shared_ptr<SpriteTexture> AnimationPlayer::create_grass_particles(const py::Vector2f &pos)
{
    auto sprite_type = this->random_choice<std::string>({"leaf1", "leaf2", "leaf3", "leaf4", "leaf5", "leaf6"});
    return std::make_shared<ParticleEffect>(pos, sprite_type, this->sprite_manager);
}

std::shared_ptr<SpriteTexture> AnimationPlayer::create_particles(const py::Vector2f &pos, const std::string &sprite_type)
{
    return std::make_shared<ParticleEffect>(pos, sprite_type, this->sprite_manager);
}

std::vector<std::shared_ptr<SpriteTexture>> AnimationPlayer::reflect_images(const std::vector<std::shared_ptr<SpriteTexture>> &frames)
{
    auto new_frames = std::vector<std::shared_ptr<SpriteTexture>>();
    for (const auto &frame : frames)
    {
        auto sprite = std::make_shared<SpriteTexture>(*(frame->texture_));
        sprite->flip(true, false);
        new_frames.push_back(sprite);
    }
    return new_frames;
}
