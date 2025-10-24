QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += core gui widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fftwstuff.cpp \
    fileloader.cpp \
    main.cpp \
    test_class_1.cpp \
    widget.cpp

HEADERS += \
    fftw3/fftw3.h \
    fftwstuff.h \
    fileloader.h \
    test_class_1.h \
    widget.h

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.txt \
    down-arrow.png \
    fftw3/COPYRIGHT \
    fftw3/fftw3.f \
    fftw3/fftw3.f03 \
    fftw3/fftw3l.f03 \
    fftw3/fftw3q.f03 \
    fftw3/libfftw3-3.dll \
    fftw3/libfftw3f-3.dll \
    fftw3/libfftw3l-3.dll \
    lessons.txt \
    note.png

INCLUDEPATH += $$PWD/fftw3
DEPENDPATH += $$PWD/fftw3

unix|win32: LIBS += -L$$PWD/fftw3/ -llibfftw3-3 -llibfftw3f-3 -llibfftw3l-3

RESOURCES += \
    resources.qrc
