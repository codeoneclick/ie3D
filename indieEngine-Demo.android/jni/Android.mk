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

SOURCE_FILES := $(wildcard $(LOCAL_PATH)/../../indie2dEngine-Demo/Sources/*.cpp)

LOCAL_MODULE := IndieEngineDemo
LOCAL_CFLAGS := -Werror -D__NDK__ -DGXX_EXPERIMENTAL_CXX0X
LOCAL_STATIC_LIBRARIES := IndieEngineCore
LOCAL_C_INCLUDES := /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Demo/Sources /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Sources /Users/sergey.sergeev/Documents/Hobby/indieEngine/indie2dEngine-Core/Libraries
LOCAL_SRC_FILES := $(SOURCE_FILES:$(LOCAL_PATH)/%=%)\
				   CGame.cpp
LOCAL_LDLIBS := -llog -lGLESv2
LOCAL_CPPFLAGS += -std=c++11
include $(BUILD_SHARED_LIBRARY)

$(call import-module,android/native_app_glue)

BUNDLEDIR := $(LOCAL_PATH)
ASSETDIR := $(BUNDLEDIR)/../../indie2dEngine-Demo/Bundle/Shaders

$(ASSETDIR): -mkdir "test"

$(ASSETDIR)/CommonGUI.vert: $(ASSETDIR) $(BUNDLEDIR)/../../indie2dEngine-Demo/Bundle/Shaders/CommonGUI.vert cp "$(BUNDLEDIR)/../../indie2dEngine-Demo/Bundle/Shaders/CommonGUI.vert" "$(ASSETDIR)/" 