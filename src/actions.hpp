#ifndef __ACTIONS_HPP__
#define __ACTIONS_HPP__

#include <memory>

class IDamagePlayer
{
public:
  virtual void fire(IDamagePlayer *) = 0;
  virtual ~IDamagePlayer() {}
};

class ICreateAttack
{
public:
  virtual void fire(ICreateAttack *) = 0;
  virtual ~ICreateAttack() {}
};

class IDestroyAttack
{
public:
  virtual void fire(IDestroyAttack *) = 0;
  virtual ~IDestroyAttack() {}
};

class ICreateMagic
{
public:
  virtual void fire(ICreateMagic *, const std::string style, int strength, int cost) = 0;
  virtual ~ICreateMagic() {}
};

class IDestroyMagic
{
public:
  virtual void fire(IDestroyMagic *) = 0;
  virtual ~IDestroyMagic() {}
};

#endif /* __ACTIONS_HPP__ */
