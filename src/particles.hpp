#ifndef __PARTICLES_HPP__
#define __PARTICLES_HPP__

#include "support.hpp"

class ParticleEffect : public SpriteTexture
{
public:
    /**
     * @brief ParticleEffect 클래스의 생성자입니다.
     *
     * 이 생성자는 주어진 위치와 스프라이트 프레임 목록을 기반으로 ParticleEffect 객체를 초기화합니다.
     * ParticleEffect는 SpriteTexture를 상속받은 클래스입니다.
     *
     * @param pos ParticleEffect의 초기 위치를 나타내는 py::Vector2f 객체입니다.
     * @param sprite_frames ParticleEffect에 사용할 스프라이트 프레임 목록으로, 공유 포인터를 담은 벡터입니다.
     *                      스프라이트 프레임 목록은 ParticleEffect가 다양한 외관을 갖게하는 데 사용됩니다.
     *                      스프라이트 프레임 목록의 각 스프라이트는 ParticleEffect 내부에서 복제되어 사용되며,
     *                      직접 수정하여 다른 객체에 영향을 미치는 것을 방지합니다.
     *
     * @note 스프라이트 프레임 목록(sprite_frames)은 ParticleEffect 내부에서 복제됩니다. 따라서, 생성자가
     * 호출되면 ParticleEffect는 스프라이트 프레임 목록의 각 스프라이트를 복제하여 사용하게 됩니다.
     * 이로 인해 스프라이트 프레임 목록의 변경이 ParticleEffect에 영향을 미치지 않습니다.
     *
     * 예시 사용법:
     *   py::Vector2f position(100.f, 200.f);
     *   std::vector<std::shared_ptr<SpriteTexture>> spriteFrames = {frame1, frame2, frame3};
     *   ParticleEffect particleEffect(position, spriteFrames);
     */
    ParticleEffect(const py::Vector2f &pos, const std::vector<std::shared_ptr<SpriteTexture>> &sprite_frames)
    {
        // 스프라이트 프레임 목록 내의 각 스프라이트를 복제하여 새로운 스프라이트 목록(frames)에 추가합니다.
        for (const auto &frame : sprite_frames)
        {
            auto cloned_frame = std::make_shared<SpriteTexture>(*frame->texture_);
            cloned_frame->get_rect({"center", pos});
            this->frames.push_back(cloned_frame);
        }

        this->sprite_type_ = "magic";
        this->frame_index = 0;
        this->animation_speed = 0.15f;

        this->update_sprite(this->frames[static_cast<int>(this->frame_index)]);
    }

    void animate()
    {
        this->frame_index += this->animation_speed;
        if (this->frame_index >= static_cast<float>(this->frames.size()))
        {
            this->frame_index = 0.f;
            // this->kill();
        }
        else
        {
            this->update_sprite(this->frames[static_cast<int>(this->frame_index)]);
        }
    }

    void update() override
    {
        this->animate();
    }

private:
    /**
     * @brief 주어진 'sprite'를 기반으로 스프라이트 텍스처와 외관을 업데이트합니다.
     *
     * 이 함수는 ParticleEffect의 스프라이트에 대한 텍스처와 외관을 'sprite'로 제공된 내용을 기반으로 업데이트합니다.
     * 'sprite'로 제공된 스프라이트의 텍스처를 ParticleEffect의 스프라이트의 텍스처로 설정하여, ParticleEffect의
     * 시각적 표현을 변경합니다. 스프라이트의 텍스처는 다른 SpriteTexture 객체들과 공유되므로, 직접 수정하는 것은
     * 같은 텍스처를 사용하는 다른 객체들에게도 의도치 않은 부작용을 초래할 수 있습니다.
     *
     * @param sprite 새로운 텍스처와 외관을 제공하는 SpriteTexture 객체에 대한 공유 포인터입니다.
     *
     * @note 'texture_' 멤버 변수에 직접 접근하여 수정하는 것은 권장되지 않으며, 캡슐화를 우회하는 방식입니다.
     * 대신, 이 함수를 사용하여 스프라이트의 텍스처를 안전하게 업데이트하고 ParticleEffect의 내부 상태의 무결성을
     * 유지해야 합니다.
     *
     * 예시 사용법:
     *   std::shared_ptr<SpriteTexture> newSprite = std::make_shared<SpriteTexture>("new_texture.png");
     *   particleEffect.update_sprite(newSprite);
     */
    void update_sprite(std::shared_ptr<SpriteTexture> sprite)
    {
        this->texture_ = sprite->texture_; // 이걸 public 이 아닌 protected 로 바꿀수가 없을까?
        this->sprite_.setTexture(*this->texture_);
        this->rect_ = sprite->rect();
    }

    float frame_index;
    float animation_speed;
    std::vector<std::shared_ptr<SpriteTexture>> frames;
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
        decltype(leaf_frames) reflected_leaf_frames;
        for (auto item = leaf_frames.begin(); item != leaf_frames.end(); ++item)
        {
            reflected_leaf_frames.push_back(this->reflect_images(*item));
        }
        this->leaf_frames.insert(
            this->leaf_frames.end(),
            reflected_leaf_frames.begin(),
            reflected_leaf_frames.end());
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
        auto new_frames = std::vector<std::shared_ptr<SpriteTexture>>();
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
