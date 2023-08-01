#include "magic.hpp"

MagicPlayer::MagicPlayer(const std::shared_ptr<AnimationPlayer> &animation_player) : animation_player(animation_player)
{
    this->sounds = {
        {"heal", this->mixer_sound("./src/audio/heal.wav")},
        {"flame", this->mixer_sound("./src/audio/Fire.wav")}};

    this->sounds["heal"].setVolume(20);
    this->sounds["flame"].setVolume(10);
}

std::vector<std::shared_ptr<SpriteTexture>> MagicPlayer::heal(std::shared_ptr<Player> player, int strength, int cost)
{
    if (player->energy() >= cost)
    {
        this->sounds["heal"].play();
        player->energy(player->energy() - cost);
        player->health(player->health() + strength);

        auto stats = player->stats();
        if (player->health() > stats["health"])
        {
            player->health(stats["health"]);
        }

        auto rect = player->rect();
        auto pos = rect.center();
        auto aura = this->animation_player->create_particles(pos, "aura");
        auto heal = this->animation_player->create_particles(pos + sf::Vector2f(0, -30), "heal");
        return {aura, heal};
    }
    else
    {
        return {};
    }
}

std::vector<std::shared_ptr<SpriteTexture>> MagicPlayer::flame(std::shared_ptr<Player> player, int cost)
{
    std::vector<std::shared_ptr<SpriteTexture>> sprites;

    if (player->energy() >= cost)
    {
        this->sounds["flame"].play();
        player->energy(player->energy() - cost);

        sf::Vector2f direction;
        auto status = player->status();
        auto see = status.substr(0, status.find('_'));
        if (!see.compare("left"))
        {
            direction.x = -1;
        }
        else if (!see.compare("right"))
        {
            direction.x = 1;
        }
        else if (!see.compare("up"))
        {
            direction.y = -1;
        }
        else if (!see.compare("down"))
        {
            direction.y = 1;
        }

        auto rect = player->rect();
        for (auto i = 1; i < 6; ++i)
        {
            auto pos = py::Vector2f();

            if (direction.x)
            { // horizontal
                auto offset_x = (direction.x * i) * TILESIZE;
                pos.x = rect.center().x + offset_x + randint(-TILESIZE / 3, TILESIZE / 3);
                pos.y = rect.center().y + randint(-TILESIZE / 3, TILESIZE / 3);
            }
            else
            { // vertical
                auto offset_y = (direction.y * i) * TILESIZE;
                pos.x = rect.center().x + randint(-TILESIZE / 3, TILESIZE / 3);
                pos.y = rect.center().y + offset_y + randint(-TILESIZE / 3, TILESIZE / 3);
            }
            auto sprite = this->animation_player->create_particles(pos, "flame");
            sprites.push_back(sprite);
        }
    }

    return sprites;
}

int MagicPlayer::randint(int from, int to)
{
    return rand() % (to - from + 1) + from;
}

sf::Sound MagicPlayer::mixer_sound(const std::string &path)
{
    sf::SoundBuffer buffer;
    auto r = buffer.loadFromFile(path);
    assert(r);
    this->sound_buffers.push_back(buffer);
    sf::Sound sound;
    sound.setBuffer(buffer);
    return sound;
}