/*
 * IndieEngineCore.cpp
 *
 *  Created on: Nov 28, 2013
 *      Author: sergey.sergeev
 */

#include <jni.h>
#include "CGameViewController_ndk.h"
#include "CGameLoopExecutor.h"

extern "C" JNIEXPORT void JNICALL Java_com_codeoneclick_indieenginedemo_CGameJNILib_start(JNIEnv * env, jobject obj)
{
	CGameViewController_ndk* controller = new CGameViewController_ndk();
}

extern "C" JNIEXPORT void JNICALL Java_com_codeoneclick_indieenginedemo_CGameJNILib_run(JNIEnv * env, jobject obj)
{
	Run();
}
