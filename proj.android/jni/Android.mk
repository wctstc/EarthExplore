LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/Logic/GlobalSchedule/GlobalSchedule.cpp \
                   ../../Classes/Logic/GlobalSchedule/IntervalSchedule.cpp \
                   ../../Classes/Logic/Lua/src/lapi.c \
                   ../../Classes/Logic/Lua/src/lauxlib.c \
                   ../../Classes/Logic/Lua/src/lbaselib.c \
                   ../../Classes/Logic/Lua/src/lcode.c \
                   ../../Classes/Logic/Lua/src/ldblib.c \
                   ../../Classes/Logic/Lua/src/ldebug.c \
                   ../../Classes/Logic/Lua/src/ldo.c \
                   ../../Classes/Logic/Lua/src/ldump.c \
                   ../../Classes/Logic/Lua/src/lfunc.c \
                   ../../Classes/Logic/Lua/src/lgc.c \
                   ../../Classes/Logic/Lua/src/linit.c \
                   ../../Classes/Logic/Lua/src/liolib.c \
                   ../../Classes/Logic/Lua/src/llex.c \
                   ../../Classes/Logic/Lua/src/lmathlib.c \
                   ../../Classes/Logic/Lua/src/lmem.c \
                   ../../Classes/Logic/Lua/src/loadlib.c \
                   ../../Classes/Logic/Lua/src/lobject.c \
                   ../../Classes/Logic/Lua/src/lopcodes.c \
                   ../../Classes/Logic/Lua/src/loslib.c \
                   ../../Classes/Logic/Lua/src/lparser.c \
                   ../../Classes/Logic/Lua/src/lstate.c \
                   ../../Classes/Logic/Lua/src/lstring.c \
                   ../../Classes/Logic/Lua/src/lstrlib.c \
                   ../../Classes/Logic/Lua/src/ltable.c \
                   ../../Classes/Logic/Lua/src/ltablib.c \
                   ../../Classes/Logic/Lua/src/ltm.c \
                   ../../Classes/Logic/Lua/src/luac.c \
                   ../../Classes/Logic/Lua/src/lundump.c \
                   ../../Classes/Logic/Lua/src/lvm.c \
                   ../../Classes/Logic/Lua/src/lzio.c \
                   ../../Classes/Logic/Lua/src/print.c \
                   ../../Classes/Logic/Main/DataManager.cpp \
                   ../../Classes/Logic/Main/GameManager.cpp \
                   ../../Classes/Logic/Main/GeneralKit.cpp \
                   ../../Classes/Logic/Main/LuaObject.cpp \
                   ../../Classes/Logic/Player/Player.cpp \
                   ../../Classes/Logic/Reward/Reward.cpp \
                   ../../Classes/Logic/System/GameSystem.cpp \
                   ../../Classes/UI/Components/Component.cpp \
                   ../../Classes/UI/MainScene/MainScene.cpp \
                   ../../Classes/UI/Animation/Animation.cpp \
                   ../../Classes/UI/ShopScene/ShopScene.cpp \
                   ../../Classes/UI/StartScene/StartScene.cpp\
                   ../../Classes/Pay/Log.cpp\
                   ../../Classes/Pay/PayClient.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes \
                    $(LOCAL_PATH)/../../Classes/Logic/CommonHeader \
                    $(LOCAL_PATH)/../../Classes/Logic/GlobalSchedule \
                    $(LOCAL_PATH)/../../Classes/Logic/Lua\include \
                    $(LOCAL_PATH)/../../Classes/Logic/Main \
                    $(LOCAL_PATH)/../../Classes/Logic/Log \
                    $(LOCAL_PATH)/../../Classes/Logic/Player \
                    $(LOCAL_PATH)/../../Classes/Logic/Reward \
                    $(LOCAL_PATH)/../../Classes/Logic/System \
                    $(LOCAL_PATH)/../../Classes/UI/Components \
                    $(LOCAL_PATH)/../../Classes/UI/MainScene \
                    $(LOCAL_PATH)/../../Classes/UI/StartScene \
                    $(LOCAL_PATH)/../../Classes/UI/ShopScene \
                    $(LOCAL_PATH)/../../Classes/UI/Animation \
                    $(LOCAL_PATH)/../../Classes/UI \
                    $(LOCAL_PATH)/../../Classes/Pay

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
