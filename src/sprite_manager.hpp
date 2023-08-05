#ifndef __SPRITE_MANAGER_HPP__
#define __SPRITE_MANAGER_HPP__

#include "support.hpp"
#include <vector>
#include <memory>

class SpriteManager
{
public:
  virtual void kill(const SpriteTexture *sprite_texture) = 0;
  virtual void kill(const std::shared_ptr<SpriteTexture> &sprite_texture) = 0;

protected:
  std::vector<std::shared_ptr<SpriteTexture>> kill_queue;
};

#endif /* __SPRITE_MANAGER_HPP__ */
