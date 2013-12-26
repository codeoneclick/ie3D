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
#include "IInputContext.h"

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

       case APP_CMD_TERM_WINDOW:
       {
    	   ANativeActivity_finish(_application->activity);
    	   exit(0);
       }
       break;

       case APP_CMD_GAINED_FOCUS:
       {
    	   g_animating = 1;
       }
       break;

       case APP_CMD_LOST_FOCUS:
       {
    	   ANativeActivity_finish(_application->activity);
    	   exit(0);
       }
       break;
    }
}

static int32_t custom_handle_input(struct android_app* _application, AInputEvent* _event)
{
    if( AInputEvent_getType(_event) == AINPUT_EVENT_TYPE_MOTION )
    {
    	i32 pointerCount = AMotionEvent_getPointerCount(_event);
    	i32 sourceId = AInputEvent_getSource(_event);

    	for(ui32 i = 0 ; i < pointerCount ; ++i)
    	{
    		i32 pointerId = AMotionEvent_getPointerId(_event, i);
    		i32 action = AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(_event);
    		i32 rawAction	= AMotionEvent_getAction(_event);

    		if(sourceId == AINPUT_SOURCE_TOUCHPAD)
    		{

    		}
    		else
    		{

    		}

    		f32 pointX = AMotionEvent_getX(_event, 0);
    		f32 pointY = AMotionEvent_getY(_event, 0);

    		if(action == AMOTION_EVENT_ACTION_POINTER_DOWN || action == AMOTION_EVENT_ACTION_POINTER_UP)
    		{
    			i32 pointerIndex = (AMotionEvent_getAction(_event) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
    			pointerId = AMotionEvent_getPointerId(_event, pointerIndex);
    		}

    		if(action == AMOTION_EVENT_ACTION_DOWN || action == AMOTION_EVENT_ACTION_POINTER_DOWN)
    		{
    			IInputContext::NativeCallTapRecognizerPressed(glm::ivec2(pointX, pointY));
    		}
    		else if( action == AMOTION_EVENT_ACTION_UP || action == AMOTION_EVENT_ACTION_POINTER_UP || action == AMOTION_EVENT_ACTION_CANCEL )
    		{
    			IInputContext::NativeCallTapRecognizerReleased(glm::ivec2(pointX, pointY));
    		}
    		else if(action == AMOTION_EVENT_ACTION_MOVE)
    		{
    			IInputContext::NativeCallTapRecognizerMoved(glm::ivec2(pointX, pointY));
    		}
    	}
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
		NDK_LOG("while loop");
		struct android_poll_source* source;
		while (ALooper_pollAll(g_animating ? 0 : -1, NULL, &events, (void**)&source) >= 0)
		{
			NDK_LOG("run loop events");
			if (source != nullptr)
			{
				source->process(_application, source);
			}

			if (_application->destroyRequested != 0)
			{
				ANativeActivity_finish(_application->activity);
				exit(0);
				return;
			}
		}
		if(g_animating == 1)
		{
			Run();
		}
	}
}
