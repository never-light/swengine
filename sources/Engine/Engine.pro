include(../Config.pri)

TEMPLATE = lib
CONFIG += staticlib c++17
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$THIRD_PARTY_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR/SDL2/include
INCLUDEPATH += $$THIRD_PARTY_DIR/pugixml/include
INCLUDEPATH += $$THIRD_PARTY_DIR/OpenGL
#INCLUDEPATH += $$SOURCES_DIR/ThirdParty

DEPENDPATH += $$THIRD_PARTY_DIR

SOURCES += \
    Exceptions/EngineRuntimeException.cpp \
    Modules/Application/BaseGameApplication.cpp \
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
    Modules/ResourceManagement/Resource.cpp \
    Modules/ResourceManagement/ResourceInstance.cpp \
    Modules/ResourceManagement/ResourceManagementModule.cpp \
    Modules/ResourceManagement/ResourceManager.cpp \
    Utility/strings.cpp \
    Utility/xml.cpp

HEADERS += \
    Exceptions/EngineRuntimeException.h \
    Modules/Application/BaseGameApplication.h \
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
    Modules/ResourceManagement/RawDataStructures.h \
    Modules/ResourceManagement/Resource.h \
    Modules/ResourceManagement/ResourceInstance.h \
    Modules/ResourceManagement/ResourceManagementModule.h \
    Modules/ResourceManagement/ResourceManager.h \
    Utility/helpers.h \
    Utility/strings.h \
    Utility/xml.h \
    types.h

include($$PWD/Modules/Graphics/Graphics.pri)

QMAKE_CXXFLAGS += -include $$PWD/Utility/helpers.h
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Werror

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Modules/Graphics/Graphics.pri
