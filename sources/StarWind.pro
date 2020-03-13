include(Config.pri)

TEMPLATE = subdirs

SUBDIRS += \
    Engine \
    Game \
    DemoApp \
    MeshTool \
    Tests

Game.depends = Engine
MeshTool.depends = Engine
Tests.depends = Engine
DemoApp.depends = Engine
