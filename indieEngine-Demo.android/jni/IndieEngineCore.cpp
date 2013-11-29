/*
 * IndieEngineCore.cpp
 *
 *  Created on: Nov 28, 2013
 *      Author: sergey.sergeev
 */

#include <jni.h>
#include "HCommon.h"
#include "CBone.h"

extern "C" JNIEXPORT void JNICALL Java_com_codeoneclick_indieenginedemo_CGameJNILib_fooo(JNIEnv * env, jobject obj)
{
	std::shared_ptr<CBone> bone_01 = std::make_shared<CBone>(0, 0);
	std::shared_ptr<CBone> bone_02 = std::make_shared<CBone>(1, 0);
	bone_01->AddChild(bone_02);
}
