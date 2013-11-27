LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := IndieEngine-Demo
LOCAL_SRC_FILES := IndieEngine-Demo.cpp

include $(BUILD_SHARED_LIBRARY)
