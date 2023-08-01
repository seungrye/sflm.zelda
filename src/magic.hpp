#ifndef __MAGIC_HPP__
#define __MAGIC_HPP__

#include "particles.hpp"
#include <SFML/Audio.hpp>
#include <memory>
#include <iostream>
#include <cassert>

class MagicPlayer
{
public:
    MagicPlayer(const std::shared_ptr<AnimationPlayer> &animation_player);
    std::vector<std::shared_ptr<SpriteTexture>> heal(std::shared_ptr<Player> player, int strength, int cost);
    std::vector<std::shared_ptr<SpriteTexture>> flame(std::shared_ptr<Player> player, int cost);

private:
    int randint(int from, int to);
    sf::Sound mixer_sound(const std::string &path);

private:
    const std::shared_ptr<AnimationPlayer> &animation_player;
    std::vector<sf::SoundBuffer> sound_buffers;
    std::map<std::string, sf::Sound> sounds;
};

#endif /* __MAGIC_HPP__ */
