#ifndef __ACTIONS_HPP__
#define __ACTIONS_HPP__

#include <memory>

class ICommand {
  public:
  virtual void invoke() = 0;
  virtual ~ICommand() {};
};


#endif /* __ACTIONS_HPP__ */
