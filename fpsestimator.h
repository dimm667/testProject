#ifndef FPSESTIMATOR_H
#define FPSESTIMATOR_H
#include <chrono>

class FpsEstimator
{
public:
    FpsEstimator() :
    initilized(false),
    frameCount(0u),
    fps(0.0f),
    lastTime()
    {}

    void update()
    {
        auto nowTime = clock.now();
        if(initilized)
        {
            frameCount++;
            std::chrono::duration<float, std::milli> delta = nowTime - lastTime;
            float deltaf = delta.count();
            if(deltaf > 100.0f)
            {
                fps = 0.9f * fps + 0.1f * ( static_cast<float>(frameCount) * 1000.0f / deltaf - fps);
                lastTime = nowTime;
                frameCount = 0;
            }
        }
        else
        {
            lastTime = nowTime;
            initilized = true;
        }
    }

    float get() const
    {
        return fps;
    }

private:
    bool initilized;
    unsigned int                            frameCount;
    float                                   fps;
    std::chrono::system_clock::time_point   lastTime;
    std::chrono::system_clock               clock;
};

#endif // FPSESTIMATOR_H
