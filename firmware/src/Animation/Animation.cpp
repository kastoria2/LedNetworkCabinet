#include "Animation.h"
#include "HexPanel.h"

Animation::Animation(const String& _displayName, HexPanel& hexPanel) : displayName(_displayName), hexPanel(hexPanel)
{}

const String& Animation::getDisplayName()
{
  return displayName;
}

void Animation::toJson(JsonDocument& doc)
{
  doc["name"] = this->displayName.c_str();
}