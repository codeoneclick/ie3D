LOCAL_PATH := $(call my-dir)

#Build module for static library

include $(CLEAR_VARS)
LOCAL_MODULE := IndieEngineCore
LOCAL_SRC_FILES := /Users/sergey.sergeev/Documents/Hobby/indieEngine/indieEngine-CoreNDK.android/obj/local/armeabi/libIndieEngineCoreNDK.a
LOCAL_EXPORT_C_INCLUDES := /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Sources /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Libraries
LOCAL_CPPFLAGS += -std=c++11
include $(PREBUILT_STATIC_LIBRARY)

#Build module for jni wrapper

include $(CLEAR_VARS)
LOCAL_MODULE := IndieEngineDemo
LOCAL_CFLAGS := -Werror
LOCAL_C_INCLUDES := /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Sources /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Libraries
LOCAL_SRC_FILES := IndieEngineCore.cpp
LOCAL_STATIC_LIBRARIES := IndieEngineCore
LOCAL_LDLIBS := -llog -lGLESv2
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)