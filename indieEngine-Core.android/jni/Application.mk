NDK_TOOLCHAIN_VERSION = 4.8
#NDK_TOOLCHAIN_VERSION = clang
APP_OPTIM := release
APP_PLATFORM := android-14
APP_STL := gnustl_static
APP_ABI := armeabi-v7a
APP_CFLAGS += -Wno-error=format-security
APP_CFLAGS += -Wno-deprecated
APP_CPPFLAGS += -std=c++11
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti

