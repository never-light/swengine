include(../Config.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Engine/release/ -lEngine
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Engine/debug/ -lEngine
else:unix: LIBS += -L$$OUT_PWD/../Engine/ -lEngine

INCLUDEPATH += $$SOURCES_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR
INCLUDEPATH += $$THIRD_PARTY_DIR/SDL2/include
INCLUDEPATH += $$THIRD_PARTY_DIR/OpenGL
INCLUDEPATH += $$THIRD_PARTY_DIR/Assimp/include
INCLUDEPATH += $$THIRD_PARTY_DIR/cxxopts/include
INCLUDEPATH += $$SOURCES_DIR/Engine

DEPENDPATH += $$THIRD_PARTY_DIR
DEPENDPATH += $$SOURCES_DIR/Engine

SOURCES += \
    MeshExporter.cpp \
    MeshImporter.cpp \
    MeshTool.cpp \
    MeshToolApplication.cpp
HEADERS += \
    \ \
    MeshExporter.h \
    MeshImporter.h \
    MeshToolApplication.h

LIBS += $$THIRD_PARTY_DIR/SDL2/lib/x64/libSDL2main.a
LIBS += $$THIRD_PARTY_DIR/SDL2/lib/x64/libSDL2.dll.a
LIBS += $$THIRD_PARTY_DIR/Assimp/lib/libassimp.dll.a

QMAKE_CXXFLAGS += -include $$PWD/../Engine/Utility/helpers.h
QMAKE_CXXFLAGS += -Wall
QMAKE_CXXFLAGS += -Werror

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/release/libEngine.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/debug/libEngine.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/release/Engine.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../Engine/debug/Engine.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../Engine/libEngine.a

DESTDIR = $$APP_BIN_DIR
