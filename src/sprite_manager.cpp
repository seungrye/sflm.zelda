#include "sprite_manager.hpp"
#include <experimental/filesystem>

void SpriteManager::import(const std::string &name, const std::string &path)
{
    this->mapped_textures[name] = {};
    for (const auto &entry :
         std::experimental::filesystem::directory_iterator(path))
    {

        if (std::experimental::filesystem::is_regular_file(entry))
        {
            const auto &path = entry.path();
            auto texture = sf::Texture();
            auto r = texture.loadFromFile(path);
            assert(r);
            this->mapped_textures[name].push_back(texture);
        }
    }
}
std::vector<std::shared_ptr<SpriteTexture>> SpriteManager::sprite_textures(const std::string &name)
{
    std::vector<std::shared_ptr<SpriteTexture>> sprites;
    for (const auto &texture : this->mapped_textures[name])
    {
        sprites.push_back(std::make_shared<SpriteTexture>(texture));
    }
    return sprites;
}

const std::vector<sf::Texture> &SpriteManager::textures(const std::string &name)
{
    return this->mapped_textures[name];
}
