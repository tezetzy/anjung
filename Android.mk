LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := AbtFixesJay
LOCAL_SRC_FILES := AbtFixes.cpp
LOCAL_CPPFLAGS += -Wall -std=c++17
LOCAL_LDLIBS := -llog -ldl
include $(BUILD_SHARED_LIBRARY)
