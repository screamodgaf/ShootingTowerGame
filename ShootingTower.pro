QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17, console

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    Particles/fireparticle.cpp \
#    Particles/fireparticlesystem.cpp \
    AutonomousAgents/agent.cpp \
    Particles/particle.cpp \
    Particles/particlesystem.cpp \
    Particles/repeller.cpp \
    ally.cpp \
    bullet.cpp \
    bulletshooting.cpp \
    enemy.cpp \
    enemyship.cpp \
    game.cpp \
    level1.cpp \
    main.cpp \
    player.cpp \
    playerdefences.cpp \
    playerweapons.cpp \
    tower.cpp \


HEADERS += \
#    Particles/fireparticle.h \
#    Particles/fireparticlesystem.h \
    AutonomousAgents/agent.h \
    Particles/particle.h \
    Particles/particlesystem.h \
    Particles/repeller.h \
    ally.h \
    bullet.h \
    bulletshooting.h \
    enemy.h \
    enemyship.h \
    game.h \
    level1.h \
    player.h \
    playerdefences.h \
    playerweapons.h \
    tower.h \


#LIBS += -LD:/Development/Boost/stage/lib -l libboost_system-mgw11-mt-d-x64-1_79


FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



#




unix|win32: LIBS += -L$$PWD/../../Boost/boost_1_79_0/stage/lib/ -llibboost_stacktrace_windbg-mgw11-mt-d-x64-1_79.dll

INCLUDEPATH += $$PWD/../../Boost/boost_1_79_0
DEPENDPATH += $$PWD/../../Boost/boost_1_79_0
