APP_PLATFORM := android-9
APP_ABI := all

NDK_TOOLCHAIN_VERSION = 4.8
APP_STL := gnustl_static
APP_CFLAGS += -Wno-error=format-security
APP_CFLAGS += -Wno-deprecated
APP_GNUSTL_FORCE_CPP_FEATURES := exceptions rtti