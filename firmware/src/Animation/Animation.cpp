#include "Animation.h"

Animation::Animation(const String& _displayName) : displayName(_displayName)
{}

const String& Animation::getDisplayName()
{
  return displayName;
}

void Animation::toJson(JsonDocument& doc)
{
  doc["name"] = this->displayName.c_str();
}