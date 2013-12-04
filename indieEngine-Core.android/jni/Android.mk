LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SOURCE_FILES := $(wildcard $(LOCAL_PATH)/../../indie2dEngine-Core/Sources/*.cpp)
SOURCE_FILES += $(wildcard $(LOCAL_PATH)/../../indie2dEngine-Core/Libraries/pugixml/*.cpp)
#SOURCE_FILES += $(wildcard $(LOCAL_PATH)/../../indie2dEngine-Core/Libraries/gcdpp/*.cpp)

LOCAL_MODULE := IndieEngine-Core
LOCAL_CFLAGS := -Werror -D__NDK__ -DGXX_EXPERIMENTAL_CXX0X
LOCAL_CFLAGS += -g
LOCAL_CFLAGS += -ggdb
LOCAL_CFLAGS += -O1
LOCAL_CPPFLAGS += -std=c++11
LOCAL_C_INCLUDES := ../indie2dEngine-Core/Sources\
	 				../indie2dEngine-Core/Libraries\
	 				../indie2dEngine-Core/Libraries/gcdpp/include\
					../indie2dEngine-Core/Libraries/jsoncpp/include\
					../indie2dEngine-Core/Libraries/curl/include\
					../indie2dEngine-Core/Libraries/ogles2/include\
					../indie2dEngine-Core/Libraries/pvrtextool/include
LOCAL_SRC_FILES := $(SOURCE_FILES:$(LOCAL_PATH)/%=%)
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_STATIC_LIBRARY)
$(call import-module,android/native_app_glue)
