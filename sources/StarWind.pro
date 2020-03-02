include(Config.pri)

TEMPLATE = subdirs

SUBDIRS += \
    Engine \
    Game \
    MeshTool \
    Tests

Game.depends = Engine
MeshTool.depends = Engine
Tests.depends = Engine
