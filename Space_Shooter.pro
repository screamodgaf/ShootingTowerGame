QT       += core gui
QT += openglwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17, console


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    Particles/fireparticle.cpp \
#    Particles/fireparticlesystem.cpp \
    Particles/particle.cpp \
    Particles/particlesystem.cpp \
    Particles/repeller.cpp \
    bullet.cpp \
    game.cpp \
    level1.cpp \
    loadresources.cpp \
    main.cpp \
    player.cpp \
    playerdefences.cpp \
    playerweapons.cpp \
    enemy.cpp \


HEADERS += \
#    Particles/fireparticle.h \
#    Particles/fireparticlesystem.h \
    Particles/particle.h \
    Particles/particlesystem.h \
    Particles/repeller.h \
    bullet.h \
    game.h \
    level1.h \
    loadresources.h \
    player.h \
    playerdefences.h \
    playerweapons.h \
    enemy.h \


#LIBS += -LD:/Development/Boost/stage/lib -l libboost_system-mgw11-mt-d-x64-1_79


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



#




#unix|win32: LIBS += -L$$PWD/../../Boost/boost_1_79_0/stage/lib/ -llibboost_stacktrace_windbg-mgw11-mt-d-x64-1_79.dll

#INCLUDEPATH += $$PWD/../../Boost/boost_1_79_0
#DEPENDPATH += $$PWD/../../Boost/boost_1_79_0

RESOURCES += \
    Resources.qrc





