LOCAL_PATH := $(call my-dir)

#Build module for static library

include $(CLEAR_VARS)
LOCAL_MODULE := IndieEngine-Core
LOCAL_SRC_FILES := ../../indieEngine-Core.android/obj/local/armeabi/libIndieEngine-Core.a
LOCAL_EXPORT_C_INCLUDES := ../../indie2dEngine-Core/Sources ../indie2dEngine-Core/Libraries
LOCAL_CPPFLAGS += -std=c++11
include $(PREBUILT_STATIC_LIBRARY)

#Build module for jni wrapper

include $(CLEAR_VARS)

SOURCE_FILES := $(wildcard $(LOCAL_PATH)/../../indie2dEngine-Demo/Sources/*.cpp)

LOCAL_MODULE := IndieEngine-Demo
LOCAL_CFLAGS := -Werror -D__NDK__ -DGXX_EXPERIMENTAL_CXX0X
LOCAL_STATIC_LIBRARIES := IndieEngine-Core
LOCAL_C_INCLUDES := ../indie2dEngine-Demo/Sources ../indie2dEngine-Core/Sources ../indie2dEngine-Core/Libraries
LOCAL_SRC_FILES := $(SOURCE_FILES:$(LOCAL_PATH)/%=%)\
				   CGame.cpp
LOCAL_LDLIBS := -llog -lGLESv2
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)