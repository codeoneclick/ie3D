/*
 * IndieEngineCore.cpp
 *
 *  Created on: Nov 28, 2013
 *      Author: sergey.sergeev
 */

#include <jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "CGameViewController_ndk.h"
#include "CGameLoopExecutor.h"
#include "CResourceAccessor.h"
#include "IGraphicsContext.h"

ui32 g_animating = 0;

static void custom_handle_cmd(struct android_app* _application, int32_t _command)
{
    switch(_command)
    {
       case APP_CMD_INIT_WINDOW:
       {
    	   IGraphicsContext::Set_AWindow(_application->window);
    	   CResourceAccessor::Set_AAssetManager(_application->activity->assetManager);
    	   CGameViewController_ndk* controller = new CGameViewController_ndk();
    	   Run();
       }
       break;
       case APP_CMD_GAINED_FOCUS:
       {
    	   g_animating = 1;
       }
       break;

       case APP_CMD_LOST_FOCUS:
       {
    	   g_animating = 0;
       }
       break;
    }
}

static int32_t custom_handle_input(struct android_app* app, AInputEvent* event)
{
    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
    	NDK_LOG("Motion Event: x %f / y %f", AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0));
    	return 1;
    }
    return 0;
}

void android_main(struct android_app* _application)
{
	app_dummy();

	int events;
	_application->onAppCmd = custom_handle_cmd;
	_application->onInputEvent = custom_handle_input;

	ANativeActivity_setWindowFlags(_application->activity, AWINDOW_FLAG_FULLSCREEN|AWINDOW_FLAG_KEEP_SCREEN_ON , 1);

	while (1)
	{
		struct android_poll_source* source;
		while (ALooper_pollAll(g_animating ? 0 : -1, NULL, &events, (void**)&source) >= 0)
		{
			if (source != nullptr)
			{
				source->process(_application, source);
			}

			if (_application->destroyRequested != 0)
			{
				return;
			}
		}
		if(g_animating == 1)
		{
			Run();
		}
	}
}
