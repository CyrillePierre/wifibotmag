######################################################################
# Automatically generated by qmake (2.01a) dim. d�c. 21 19:31:51 2014
######################################################################

CONFIG += qt
QT += core gui widgets network

TEMPLATE = app
TARGET = driving
DEPENDPATH += . include src
INCLUDEPATH += . include

release: DESTDIR = release
debug:   DESTDIR = debug

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

# Input
HEADERS += robot_wifibot.h \
           include/AbstractCommand.h \
           include/ConfigDialog.h \
           include/Configkeyboardlabel.h \
           include/Configkeyboardpage.h \
           include/Configmodel.h \
           include/Configwifibotpage.h \
           include/Configxmlreader.h \
           include/Configxmlwriter.h \
           include/ConnectDialog.h \
           include/Controler.h \
           include/InterfaceVideo.h \
           include/Joystick.h \
           include/Keyboard.h \
           include/Receiver_robot.h \
           include/Receiver_video.h \
           include/Sender_robot.h
SOURCES += main.cpp \
           robot_wifibot.cpp \
           src/AbstractCommand.cpp \
           src/ConfigDialog.cpp \
           src/Configkeyboardlabel.cpp \
           src/Configkeyboardpage.cpp \
           src/Configmodel.cpp \
           src/Configwifibotpage.cpp \
           src/Configxmlreader.cpp \
           src/Configxmlwriter.cpp \
           src/ConnectDialog.cpp \
           src/Controler.cpp \
           src/InterfaceVideo.cpp \
           src/Joystick.cpp \
           src/Keyboard.cpp \
           src/Receiver_robot.cpp \
           src/Receiver_video.cpp \
           src/Sender_robot.cpp
RESOURCES += robot_wifibot.qrc
