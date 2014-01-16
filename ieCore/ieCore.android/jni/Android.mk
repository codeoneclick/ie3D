LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

SOURCE_FILES := $(wildcard $(LOCAL_PATH)/../../Sources/*.cpp)
SOURCE_FILES += $(wildcard $(LOCAL_PATH)/../../Libraries/pugixml/*.cpp)
#SOURCE_FILES += $(wildcard $(LOCAL_PATH)/../../Libraries/gcdpp/*.cpp)

LOCAL_MODULE := ieCore
LOCAL_CFLAGS := -Werror -D__NDK__ -DGXX_EXPERIMENTAL_CXX0X
LOCAL_CFLAGS += -O2
LOCAL_CFLAGS += -DNDEBUG 
#LOCAL_CFLAGS += -g
#LOCAL_CFLAGS += -ggdb
#LOCAL_CFLAGS += -O1
LOCAL_CPPFLAGS += -std=c++11
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Sources\
	 				$(LOCAL_PATH)/../../Libraries\
	 				$(LOCAL_PATH)/../../Libraries/gcdpp/include\
					$(LOCAL_PATH)/../../Libraries/jsoncpp/include\
					$(LOCAL_PATH)/../../Libraries/curl/include\
					$(LOCAL_PATH)/../../Libraries/ogles2/include\
					$(LOCAL_PATH)/../../Libraries/pvrtextool/include
LOCAL_SRC_FILES := $(SOURCE_FILES:$(LOCAL_PATH)/%=%)
LOCAL_STATIC_LIBRARIES := android_native_app_glue
include $(BUILD_STATIC_LIBRARY)
$(call import-module,android/native_app_glue)
