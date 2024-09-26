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

public:
/**
 * @brief path에 있는 이미지들을 name 이라는 그룹으로 저장
 * 
 * @param name asset 그룹 이름
 * @param path 이미지(들)가 들어있는 경로
 */
  void import(const std::string &name, const std::string &path);
  std::vector<std::shared_ptr<SpriteTexture>> sprite_textures(const std::string& name);
  /**
   * @brief name 그룹에 매칭되는 texture 리스트를 반환
   * 
   * @param name asset 그룹 이름
   * @return const std::vector<sf::Texture>& texture(이미지) 리스트
   */
  const std::vector<sf::Texture>& textures(const std::string& name);

protected:
  std::vector<std::shared_ptr<SpriteTexture>> kill_queue;
  std::map<std::string, std::vector<sf::Texture>> mapped_textures;
};

#endif /* __SPRITE_MANAGER_HPP__ */
