#include "pygame_adapter.hpp"
#include "support.hpp"
#include <memory>
#include <vector>

template <typename T>
T __spritecollide(const std::shared_ptr<SpriteTexture> &sprite, const T &sprite_list)
{
    auto collision_sprites = T();

    for (const auto &candidate : sprite_list)
    {
        auto intersects = sprite->get_global_bounds().intersects(candidate->get_global_bounds());
        if (intersects)
        {
            collision_sprites.push_back(candidate);
        }
    }

    return collision_sprites;
}

std::vector<std::shared_ptr<SpriteTexture>> spritecollide(const std::shared_ptr<SpriteTexture> &sprite, const std::vector<std::shared_ptr<SpriteTexture>> &sprite_list)
{
    return __spritecollide<std::vector<std::shared_ptr<SpriteTexture>>>(sprite, sprite_list);
}

std::list<std::shared_ptr<SpriteTexture>> spritecollide(const std::shared_ptr<SpriteTexture> &sprite, const std::list<std::shared_ptr<SpriteTexture>> &sprite_list)
{
    return __spritecollide<std::list<std::shared_ptr<SpriteTexture>>>(sprite, sprite_list);
}
