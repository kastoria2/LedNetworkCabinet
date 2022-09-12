#ifndef __TWINKLE_ANIMATION_H__
#define __TWINKLE_ANIMATION_H__

#include "Animation.h"

class HexPanel;

class TwinkleAnimation : public Animation
{
public:
    class TwinkleData
    {
    public:
        TwinkleData(int index = -1, int startTime_ms = 0, int duration_ms = 0);
    
        int endTime_ms();
        float lifePercent(int time_ms);

        int index;
        int startTime_ms;
        int duration_ms;
    };


    TwinkleAnimation(HexPanel& hexPanel, int twinkleCount = 10);
    ~TwinkleAnimation();

    virtual void update(const InputParams& inputParams);

private:
    void updateTwinkleData(int time_ms);
    int getNextIndex();

    TwinkleData** twinkleData;
    int twinkleCount;

    int* shuffledIndices;
    int shuffledIndicesCount;

    int nextIndex;
};

#endif  // #ifndef __TWINKLE_ANIMATION_H__