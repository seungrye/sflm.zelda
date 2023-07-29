#ifndef __SPRITE_MANAGER_HPP__
#define __SPRITE_MANAGER_HPP__

#include "support.hpp"
#include "ysort_camera_group.hpp"

class DeferredSpriteManager {
    public:
    virtual void deferred_kill(const SpriteTexture *sprite_texture) = 0;
    virtual void deferred_kill(const std::shared_ptr<SpriteTexture> &sprite_texture) = 0;
};

class SpriteManager : public DeferredSpriteManager
{
public:
    SpriteManager(YSortCameraGroup &ysort_camera_group,
                  std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites,
                  std::list<std::shared_ptr<SpriteTexture>> &attackable_sprites,
                  std::list<std::shared_ptr<SpriteTexture>> &attack_sprites)
        : ysort_camera_group_(ysort_camera_group),
          obstacle_sprites(obstacle_sprites),
          attackable_sprites(attackable_sprites),
          attack_sprites(attack_sprites)
    {
    }
    void deferred_kill(const SpriteTexture *sprite_texture) override
    {
        auto found = std::find_if(ysort_camera_group_.sprites.begin(),
                                  ysort_camera_group_.sprites.end(),
                                  [sprite_texture](std::shared_ptr<SpriteTexture> item)
                                  { return item.get() == sprite_texture; });
        this->kill_list.push_back(*found);
    }
    void deferred_kill(const std::shared_ptr<SpriteTexture> &sprite_texture) override
    {
        this->deferred_kill(sprite_texture.get());
    }

    void kill() {
        for (const auto& sprite: this->kill_list) {
            this->ysort_camera_group_.remove(sprite);
            this->obstacle_sprites.remove(sprite);
            this->attack_sprites.remove(sprite);
            this->attackable_sprites.remove(sprite);
        }
        kill_list.clear();
    }
private:
    std::vector<std::shared_ptr<SpriteTexture>> kill_list;

private:
    YSortCameraGroup &ysort_camera_group_;
    std::list<std::shared_ptr<SpriteTexture>> &obstacle_sprites;
    std::list<std::shared_ptr<SpriteTexture>> &attackable_sprites;
    std::list<std::shared_ptr<SpriteTexture>> &attack_sprites;
};

#endif /* __SPRITE_MANAGER_HPP__ */
