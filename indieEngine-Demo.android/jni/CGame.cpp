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

/*extern "C" JNIEXPORT void JNICALL Java_com_codeoneclick_indieenginedemo_CGameJNILib_start(JNIEnv * env, jobject obj)
{
	CGameViewController_ndk* controller = new CGameViewController_ndk();
}

extern "C" JNIEXPORT void JNICALL Java_com_codeoneclick_indieenginedemo_CGameJNILib_run(JNIEnv * env, jobject obj)
{
	Run();
}*/

int g_animating = 0;

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
			NDK_LOG("run loop started");
			g_animating = 1;
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
	NDK_LOG("START");
	app_dummy();

	int events;
	_application->onAppCmd = custom_handle_cmd;
	_application->onInputEvent = custom_handle_input;

	while (true)
	{
		NDK_LOG("while loop");
		struct android_poll_source* source;
		while (ALooper_pollAll(-1, NULL, &events, (void**)&source) >= 0)
		{
			NDK_LOG("run loop events");
			if (source != nullptr)
			{
				source->process(_application, source);
			}

			if(g_animating == 1)
			{
				NDK_LOG("run loop executed");

			}

			if (_application->destroyRequested != 0)
			{
				NDK_LOG("We are exiting");
				return;
			}
		}
	}
}
