#include "TwinkleAnimation.h"
#include "HexPanel.h"
#include "utils.h"

TwinkleAnimation::TwinkleData::TwinkleData(
    int index /*=-1*/, 
    int startTime_ms /*=0*/, 
    int duration_ms /*=0*/) : index(index), startTime_ms(startTime_ms), duration_ms(duration_ms)
{}
    
int TwinkleAnimation::TwinkleData::endTime_ms()
{
    return startTime_ms + duration_ms;
}

float TwinkleAnimation::TwinkleData::lifePercent(int time_ms)
{
    float percent = float(time_ms - startTime_ms) / duration_ms;
    return clamp(percent, 0.0, 1.0);
}

TwinkleAnimation::TwinkleAnimation(
    HexPanel& hexPanel, 
    int twinkleCount /*=10*/) : Animation("Twinkle", hexPanel), twinkleCount(twinkleCount), nextIndex(0)
{
    twinkleData = new TwinkleData*[twinkleCount];
    for(int i = 0; i < twinkleCount; i++)
    {
        twinkleData[i] = new TwinkleData();
    }

    // Init the randomly oreder list of indices.
    shuffledIndicesCount = this->hexPanel.getLedCount();
    shuffledIndices = new int[shuffledIndicesCount];

    for(int i = 0; i < shuffledIndicesCount; i++)
    {
        shuffledIndices[i] = i;
    }
    shuffle<int>(shuffledIndices, shuffledIndicesCount);
}

TwinkleAnimation::~TwinkleAnimation()
{
    delete[] twinkleData;
    twinkleData = nullptr;

    delete[] shuffledIndices;
    shuffledIndices = nullptr;
}

void TwinkleAnimation::updateTwinkleData(int time_ms)
{
    for(int i = 0; i < twinkleCount; i++)
    {
        TwinkleData* twinkle = twinkleData[i];
        if(twinkle->endTime_ms() < time_ms)
        {
            twinkle->index = getNextIndex();
            twinkle->startTime_ms = time_ms;
            twinkle->duration_ms = random(2000, 10000);
        }
    }
}

int TwinkleAnimation::getNextIndex()
{
    if(nextIndex >= this->shuffledIndicesCount)
    {
        shuffle<int>(shuffledIndices, shuffledIndicesCount);
        nextIndex = 0;
    }

    return shuffledIndices[nextIndex++];
}

void TwinkleAnimation::update(const InputParams& inputParams)
{
    // self.updateTwinkleData(inputParams)
    updateTwinkleData(inputParams.currentTime_ms);

    // for ledOut in self.hexPanel.ledStrip:
    for (int i = 0; i < this->hexPanel.getLedCount(); i++)
    {
        LedOut &ledOut = this->hexPanel.getLedStrip()[i];

        // ledOut.color = inputParams.color
        ledOut.color = inputParams.color;

        // for twinkle in self.twinkleData:
        for(int j = 0; j < twinkleCount; j++)
        {
            TwinkleData* twinkle = twinkleData[j];
            //     if twinkle.index != ledOut.index:
            //         continue
            if (twinkle->index != i)
            {
                continue;
            }

            //     lifePercent = twinkle.lifePercent(inputParams.currentTime_ms)
            //     lifeDistance = 1.0 - (2*math.fabs(0.5 - lifePercent))

            float lifePercent = twinkle->lifePercent(inputParams.currentTime_ms);
            float lifeDistance = 1.0 - (2 * fabs(0.5 - lifePercent));

            //     ledOut.color = blendComponents(inputParams.bgColor, inputParams.color, lifeDistance)
            ledOut.color = blendComponents(inputParams.bgColor, inputParams.color, lifeDistance);
        }
    }
}