LOCAL_PATH := $(call my-dir)

#Build module for static library

include $(CLEAR_VARS)
LOCAL_MODULE := ieCore
LOCAL_SRC_FILES := ../../../ieCore/ieCore.android/obj/local/armeabi-v7a/libieCore.a
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/../../../ieCore/Sources $(LOCAL_PATH)/../../../ieCore/Libraries
LOCAL_CFLAGS += -O2
LOCAL_CFLAGS += -DNDEBUG 
LOCAL_CPPFLAGS += -std=c++11
include $(PREBUILT_STATIC_LIBRARY)

#Build module for jni wrapper

include $(CLEAR_VARS)

SOURCE_FILES := $(wildcard $(LOCAL_PATH)/../../Sources/*.cpp)

LOCAL_MODULE := ieDemo
LOCAL_CFLAGS := -Werror -D__NDK__ -DGXX_EXPERIMENTAL_CXX0X
LOCAL_CFLAGS += -O2
LOCAL_CFLAGS += -DNDEBUG 
#LOCAL_CFLAGS += -g
#LOCAL_CFLAGS += -ggdb
#LOCAL_CFLAGS += -O1
LOCAL_STATIC_LIBRARIES := ieCore android_native_app_glue
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Sources $(LOCAL_PATH)/../../Sources $(LOCAL_PATH)/../../Libraries
LOCAL_SRC_FILES := $(SOURCE_FILES:$(LOCAL_PATH)/%=%)\
				   CGame.cpp
LOCAL_LDLIBS := -llog -lEGL -lGLESv2 -landroid 
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)