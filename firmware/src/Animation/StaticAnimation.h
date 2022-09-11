#ifndef __STATIC_ANIMATION_H__
#define __STATIC_ANIMATION_H__

#include "Animation.h"

class HexPanel;

class StaticAnimation : public Animation
{
public:
  StaticAnimation(HexPanel& hexPanel);
  virtual void update(const InputParams& inputParams);
};

#endif // #ifndef __STATIC_ANIMATION_H__