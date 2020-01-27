include(../Config.pri)

TEMPLATE = lib
CONFIG += staticlib c++1z
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$THIRD_PARTY_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR/SDL2/include
INCLUDEPATH += $$THIRD_PARTY_DIR/pugixml/include
INCLUDEPATH += $$THIRD_PARTY_DIR/OpenGL
#INCLUDEPATH += $$SOURCES_DIR/ThirdParty

DEPENDPATH += $$THIRD_PARTY_DIR

PRECOMPILED_HEADER = precompiled.h

precompile_header:!isEmpty(PRECOMPILED_HEADER) {
    DEFINES += USE_PRECOMPILED_HEADERS
}

SOURCES += \
    Exceptions/EngineRuntimeException.cpp \
    Modules/Application/BaseGameApplication.cpp \
    Modules/Application/GameConsole.cpp \
    Modules/ECS/GameSystemsGroup.cpp \
    Modules/Input/InputEvents.cpp \
    Modules/Input/InputModule.cpp \
    Modules/Input/InputSystem.cpp \
    Modules/Math/Rect.cpp \
    Modules/Math/RectF.cpp \
    Modules/ECS/BaseComponentInstance.cpp \
    Modules/ECS/GameObject.cpp \
    Modules/ECS/GameObjectsSequentialIterator.cpp \
    Modules/ECS/GameObjectsSequentialView.cpp \
    Modules/ECS/GameSystem.cpp \
    Modules/ECS/GameWorld.cpp \
    Modules/Math/geometry.cpp \
    Modules/Math/matrices.cpp \
    Modules/ResourceManagement/RawDataStructures.cpp \
    Modules/ResourceManagement/Resource.cpp \
    Modules/ResourceManagement/ResourceInstance.cpp \
    Modules/ResourceManagement/ResourceManagementModule.cpp \
    Modules/ResourceManagement/ResourceManager.cpp \
    Modules/ScreenManagement/Screen.cpp \
    Modules/ScreenManagement/ScreenManager.cpp \
    Utility/files.cpp \
    Utility/strings.cpp \
    Utility/xml.cpp \
    precompiled.cpp

HEADERS += \
    Exceptions/EngineRuntimeException.h \
    Modules/Application/BaseGameApplication.h \
    Modules/Application/GameConsole.h \
    Modules/ECS/GameSystemsGroup.h \
    Modules/Input/InputActions.h \
    Modules/Input/InputEvents.h \
    Modules/Input/InputModule.h \
    Modules/Input/InputSystem.h \
    Modules/Math/Rect.h \
    Modules/Math/RectF.h \
    Modules/ECS/BaseComponentInstance.h \
    Modules/ECS/BaseEventsListener.h \
    Modules/ECS/ComponentHandle.h \
    Modules/ECS/ComponentInstance.h \
    Modules/ECS/ECS.h \
    Modules/ECS/EventsListener.h \
    Modules/ECS/GameObject.h \
    Modules/ECS/GameObjectImpl.h \
    Modules/ECS/GameObjectsComponentsIterator.h \
    Modules/ECS/GameObjectsComponentsIteratorImpl.h \
    Modules/ECS/GameObjectsComponentsView.h \
    Modules/ECS/GameObjectsComponentsViewImpl.h \
    Modules/ECS/GameObjectsSequentialIterator.h \
    Modules/ECS/GameObjectsSequentialView.h \
    Modules/ECS/GameSystem.h \
    Modules/ECS/GameWorld.h \
    Modules/Math/geometry.h \
    Modules/Math/matrices.h \
    Modules/ResourceManagement/RawDataStructures.h \
    Modules/ResourceManagement/Resource.h \
    Modules/ResourceManagement/ResourceInstance.h \
    Modules/ResourceManagement/ResourceManagementModule.h \
    Modules/ResourceManagement/ResourceManager.h \
    Modules/ScreenManagement/Screen.h \
    Modules/ScreenManagement/ScreenManager.h \
    Utility/Bitmask.h \
    Utility/files.h \
    Utility/strings.h \
    Utility/xml.h \
    precompiled.h \
    swdebug.h \
    types.h

include($$PWD/Modules/Graphics/Graphics.pri)

#QMAKE_CXXFLAGS += -include $$PWD/Utility/helpers.h
win32-g++: QMAKE_CXXFLAGS += -Wall
win32-g++: QMAKE_CXXFLAGS += -Werror
win32-g++: QMAKE_CXXFLAGS += -Wno-unknown-pragmas

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Modules/Graphics/Graphics.pri
