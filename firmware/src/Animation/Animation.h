#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include <Arduino.h>
#include <ArduinoJson.h>

#include "LedLocations.h"
#include "InputParams.h"

class HexPanel;

class Animation
{
public:
  Animation(const String& _displayName, HexPanel& hexPanel);

  const String& getDisplayName();

  virtual void update(const InputParams& inputParams) = 0;
  virtual void toJson(JsonDocument& doc);

protected:

  HexPanel& hexPanel;

private:
  String displayName;
};

#endif  // #ifndef __ANIMATION_H__