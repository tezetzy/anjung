LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := FixesJiggle
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS += -Wall -std=c++17
LOCAL_LDLIBS := -llog -ldl
include $(BUILD_SHARED_LIBRARY)
