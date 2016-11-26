#-------------------------------------------------
#
# Project created by QtCreator 2016-11-21T16:29:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Egyptian-Currency-Detector
TEMPLATE = app


SOURCES += \
    ../../Source-Code/circle.cpp \
    ../../Source-Code/circleandrectangledetector.cpp \
    ../../Source-Code/line.cpp \
    ../../Source-Code/main.cpp \
    ../../Source-Code/mainwindow.cpp

HEADERS  += \
    ../../Source-Code/circle.h \
    ../../Source-Code/circleandrectangledetector.h \
    ../../Source-Code/line.h \
    ../../Source-Code/mainwindow.h

LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

FORMS += \
    ../../Source-Code/mainwindow.ui
