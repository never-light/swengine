include(../Config.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Core/GameApplication.cpp \
        main.cpp


HEADERS += \
    Core/GameApplication.h

QMAKE_CXXFLAGS += -include $$PWD/../Engine/Utility/helpers.h
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Werror

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$SOURCES_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR/SDL2/include
INCLUDEPATH += $$THIRD_PARTY_DIR/OpenGL
INCLUDEPATH += $$SOURCES_DIR/Engine
#INCLUDEPATH += $$SOURCES_DIR/ThirdParty

DEPENDPATH += $$THIRD_PARTY_DIR
DEPENDPATH += $$SOURCES_DIR/Engine

LIBS += $$THIRD_PARTY_DIR/SDL2/lib/x64/SDL2main.lib
LIBS += $$THIRD_PARTY_DIR/SDL2/lib/x64/SDL2.lib
#LIBS += $$APP_BUILD_DIR/libraries/libspdlog.a

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/release/libEngine.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/debug/libEngine.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/release/Engine.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/debug/Engine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Engine/libEngine.a

DESTDIR = $$APP_BIN_DIR
