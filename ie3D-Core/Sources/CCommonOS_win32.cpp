#include "CCommonOS.h"

#if defined(__WIN32__)

std::string Get_BundlePath(void)
{
    return "../../../indieEngine.win32.Resources/";
};

ui32 Get_ScreenWidth(void)
{
    return 640;
};

ui32 Get_ScreenHeight(void)
{
    return 480;
};

f32 Get_Random(f32 _minValue, f32 _maxValue)
{
    return 0.0f;
};

#endif