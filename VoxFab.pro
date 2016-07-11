# ----------------------------------------------------------------------------
# The following information is required for Compiling with Qt.
# Please revise path to some libraries and some options for your environment.
# ----------------------------------------------------------------------------

TEMPLATE = app
TARGET = VoxFab
DESTDIR = release
QT += core gui xml opengl
QMAKE_CXXFLAGS += -std=c++0x -mmacosx-version-min=10.9
QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
CONFIG += debug
DEFINES += QT_XML_LIB QT_OPENGL_LIB USE_ZLIB_COMPRESSION USE_OPEN_GL QT_DLL PREC_MED
INCLUDEPATH += . \
    ./../Voxelyze \
    ../Voxelyze \
    ./GeneratedFiles/$(Configuration) \
    ./GeneratedFiles \
    ./../Utils \
    /usr/local/Cellar/qwt/6.1.2/lib/qwt.framework/Headers \
    /usr/local/Cellar/qt/4.8.7_1/lib/QtXml.framework/Versions/4/Headers \
    /usr/local/opt/eigen/include/eigen3 \
    /usr/local/Cellar/glfw3/3.1.2/include

LIBS += -framework OpenGL \
     -framework GLUT \
     -framework qwt \
     -framework QtXml \
     -F/usr/local/Cellar/qwt/6.1.2/lib/ \
     -L/usr/local/Cellar/qt/4.8.7_1/lib/QtXml.framework/Versions/4/ \
     /usr/local/Cellar/assimp/3.1.1/lib/libassimp.3.1.1.dylib \
#    -lOpenGL32 \
    #-lGL \
#   -lglu32 \
#    -lglut \
#    -lGLU \
    -lz \
    -lm

DEPENDPATH += .
MOC_DIR += ./GeneratedFiles/release
OBJECTS_DIR += release
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(VoxFab.pri)
