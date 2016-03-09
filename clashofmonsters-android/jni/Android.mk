LOCAL_PATH := $(call my-dir)
CLASSES_PATH := ../Application/Classes
LIBRARIES_PATH := ../Application/Libraries
LIBMONSTERS_PATH := ../Application/Libraries/libmonsters

include $(CLEAR_VARS)

LOCAL_MODULE := piratesvsninjas

LOCAL_MODULE_FILENAME := libpiratesvsninjas

LOCAL_C_INCLUDES := "$(LOCAL_PATH)/$(CLASSES_PATH)" \
                    "$(LOCAL_PATH)/$(LIBMONSTERS_PATH)" \
                    "$(LOCAL_PATH)/$(LIBMONSTERS_PATH)/protobuf" \
                    "$(LOCAL_PATH)/$(LIBMONSTERS_PATH)/csvpp" \
                    "$(LOCAL_PATH)/$(LIBMONSTERS_PATH)/base64/include" \
                    "$(LOCAL_PATH)/$(LIBRARIES_PATH)" \
                    "$(LOCAL_PATH)/$(LIBRARIES_PATH)/boost/include"

LOCAL_CPPFLAGS += -DPB_NO_EXCEPTIONS
#LOCAL_CFLAGS += -g

LOCAL_SRC_FILES := main.cpp \
                   $(CLASSES_PATH)/AppDelegate.cpp \
                   $(CLASSES_PATH)/AvatarNode.cpp \
                   $(CLASSES_PATH)/BarNode.cpp \
                   $(CLASSES_PATH)/CCMenuItemSpriteTouch.cpp \
                   $(CLASSES_PATH)/CCRemoveFromParentAction.cpp \
                   $(CLASSES_PATH)/ChooseAvatarLayer.cpp \
                   $(CLASSES_PATH)/ChooseBigUnitsLayer.cpp \
                   $(CLASSES_PATH)/ChooseSmallUnitsLayer.cpp \
                   $(CLASSES_PATH)/GameConfigurationScreen.cpp \
                   $(CLASSES_PATH)/GameFieldNode.cpp \
                   $(CLASSES_PATH)/GameHUDLayer.cpp \
                   $(CLASSES_PATH)/GameOverScreen.cpp \
                   $(CLASSES_PATH)/LargeUnitMenuItem.cpp \
                   $(CLASSES_PATH)/LoadingIndicatorLayer.cpp \
                   $(CLASSES_PATH)/MainMenu.cpp \
                   $(CLASSES_PATH)/MonotonicTimer.cpp \
                   $(CLASSES_PATH)/NavigationMenuLayer.cpp \
                   $(CLASSES_PATH)/NextRoundLayer.cpp \
                   $(CLASSES_PATH)/PlayerCaption.cpp \
                   $(CLASSES_PATH)/PlayScreen.cpp \
                   $(CLASSES_PATH)/PlayScreenProcessingLayer.cpp \
                   $(CLASSES_PATH)/Session.cpp \
                   $(CLASSES_PATH)/Settings.cpp \
                   $(CLASSES_PATH)/SettingsScreen.cpp \
                   $(CLASSES_PATH)/SmallUnitMenuNode.cpp \
                   $(CLASSES_PATH)/SoundManager.cpp \
                   $(CLASSES_PATH)/StrengthIndicatorsLayer.cpp \
                   $(CLASSES_PATH)/TutorialScreen.cpp \
                   $(CLASSES_PATH)/UnitNode.cpp \
                   $(CLASSES_PATH)/User.cpp \
                   $(CLASSES_PATH)/Utils.cpp \
                   $(CLASSES_PATH)/WaitingForOpponentLayer.cpp \
                   $(CLASSES_PATH)/GoogleAnalytics.cpp \
                   $(CLASSES_PATH)/ChartboostManager.cpp \
                   $(CLASSES_PATH)/FacebookSharing.cpp \
                   $(CLASSES_PATH)/BackgroundJob.cpp \
                   $(CLASSES_PATH)/XTLayer.cpp \
                   $(CLASSES_PATH)/AndroidNDKUtils.cpp \
                   $(LIBMONSTERS_PATH)/AI.cpp \
                   $(LIBMONSTERS_PATH)/AttackFormation.cpp \
                   $(LIBMONSTERS_PATH)/csvpp/csvpp.cpp \
                   $(LIBMONSTERS_PATH)/csvpp/stringhelper.cpp \
                   $(LIBMONSTERS_PATH)/DeserializeUnitsIndex.cpp \
                   $(LIBMONSTERS_PATH)/Field.cpp \
                   $(LIBMONSTERS_PATH)/Fixed.cpp \
                   $(LIBMONSTERS_PATH)/Game.cpp \
                   $(LIBMONSTERS_PATH)/GameUnits.cpp \
                   $(LIBMONSTERS_PATH)/Player.cpp \
                   $(LIBMONSTERS_PATH)/PlayerAction.cpp \
                   $(LIBMONSTERS_PATH)/protobuf/Buffer.cpp \
                   $(LIBMONSTERS_PATH)/protobuf/Message.cpp \
                   $(LIBMONSTERS_PATH)/RandomGenerator.cpp \
                   $(LIBMONSTERS_PATH)/SerializeUnitsIndex.cpp \
                   $(LIBMONSTERS_PATH)/Unit.cpp \
                   $(LIBMONSTERS_PATH)/base64/src/cdecode.c \
                   $(LIBMONSTERS_PATH)/base64/src/cencode.c

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
#$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
#$(call import-module,extensions)
#$(call import-module,external/Box2D)
#$(call import-module,external/chipmunk)
