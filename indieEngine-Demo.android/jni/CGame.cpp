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

static void custom_handle_cmd(struct android_app* app, int32_t cmd)
{
    switch(cmd)
    {
       case APP_CMD_INIT_WINDOW:
        NDK_LOG("App Init Window");
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

class CShared : public std::enable_shared_from_this<CShared>
{
public:
	void Foo()
	{

		NDK_LOG("Foo");
		std::shared_ptr<CShared> ptr = shared_from_this();
		if(ptr == nullptr)
		{
			NDK_LOG("FAIL");
		}
	};
};

void android_main(struct android_app* state)
{
	NDK_LOG("START");
	app_dummy();

	std::shared_ptr<CShared> a = std::make_shared<CShared>();
	a->Foo();

	int events;
	state->onAppCmd = custom_handle_cmd;
	state->onInputEvent = custom_handle_input;

	IGraphicsContext::Set_AWindow(state->window);
	CResourceAccessor::Set_AAssetManager(state->activity->assetManager);
	CGameViewController_ndk* controller = new CGameViewController_ndk();

	while (true)
	{
		struct android_poll_source* source;
		while (ALooper_pollAll(-1, NULL, &events, (void**)&source) >= 0)
		{
			if (source != nullptr)
			{
				source->process(state, source);
			}

			if (state->destroyRequested != 0)
			{
				NDK_LOG("We are exiting");
				return;
			}
		}
	}
}
