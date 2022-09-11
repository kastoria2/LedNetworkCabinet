#ifndef _RADIATE_ANIMATION_H_
#define _RADIATE_ANIMATION_H_

#include "Animation.h"

class HexPanel;

class RadiateAnimation : public Animation
{
public:
  RadiateAnimation(HexPanel& hexPanel);
  virtual void update(const InputParams& inputParams);

private:
};

#endif  // #ifndef _RADIATE_ANIMATION_H_