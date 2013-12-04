APP_PLATFORM := android-9
APP_ABI := armeabi

NDK_TOOLCHAIN_VERSION = 4.8
#NDK_TOOLCHAIN_VERSION = clang
APP_STL := gnustl_static
APP_CFLAGS += -Wno-error=format-security
APP_CFLAGS += -Wno-deprecated
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti
APP_CPPFLAGS += -std=c++11
APP_OPTIM := debug