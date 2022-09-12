#ifndef __UTILS_H__
#define __UTILS_H__

#include <Arduino.h>

float distance(const float a[], const float b[]);
float clamp(float v, float min, float max);

float blend(float a, float b, float t);

void argb2components(uint32_t argb, uint8_t* components);
uint32_t components2argb(uint8_t* components);

uint32_t blendComponents(uint32_t a, uint32_t b, float factor);

template<class T> void shuffle(T* array, int count)
{
    for(int i = 0; i < count; i++)
    {
        T tmp = array[i];

        int target = random(0, count);
        array[i] = array[target];
        array[target] = tmp;
    }
}

#endif  // #ifndef __UTILS_H__