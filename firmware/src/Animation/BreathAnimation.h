#ifndef __BREATH_ANIMATION_H__
#define __BREATH_ANIMATION_H__

#include "Animation.h"

class HexPanel;

class BreathAnimation : public Animation
{
public:
  BreathAnimation(HexPanel& hexPanel);
  virtual void update(const InputParams& inputParams);
};

#endif // #ifndef __BREATH_ANIMATION_H__