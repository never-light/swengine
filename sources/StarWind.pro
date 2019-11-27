include(Config.pri)

TEMPLATE = subdirs

SUBDIRS += \
    Engine \
    Game \
    MeshTool

Game.depends = Engine
MeshTool.depends = Engine
