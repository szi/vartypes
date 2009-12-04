#the application entry point:
SOURCES += src/example.cpp

#the main window:
SOURCES += src/mainwindow.cpp
HEADERS += src/mainwindow.h

#all files below belong to the vartype system:
SOURCES += ../vartypes/VarTypes.cpp
HEADERS += ../vartypes/VarTypes.h

SOURCES += ../vartypes/VarTypesFactory.cpp
HEADERS += ../vartypes/VarTypesFactory.h

SOURCES += ../vartypes/VarTypesInstance.cpp
HEADERS += ../vartypes/VarTypesInstance.h

SOURCES += ../vartypes/VarXML.cpp
HEADERS += ../vartypes/VarXML.h

SOURCES += ../vartypes/VarBase64.cpp
HEADERS += ../vartypes/VarBase64.h

SOURCES += ../vartypes/VarNotifier.cpp
HEADERS += ../vartypes/VarNotifier.h

SOURCES += ../vartypes/primitives/VarVal.cpp
HEADERS += ../vartypes/primitives/VarVal.h

SOURCES += ../vartypes/primitives/VarType.cpp
HEADERS += ../vartypes/primitives/VarType.h

SOURCES += ../vartypes/primitives/VarBool.cpp
HEADERS += ../vartypes/primitives/VarBool.h

SOURCES += ../vartypes/primitives/VarDouble.cpp
HEADERS += ../vartypes/primitives/VarDouble.h

SOURCES += ../vartypes/primitives/VarInt.cpp
HEADERS += ../vartypes/primitives/VarInt.h

SOURCES += ../vartypes/primitives/VarList.cpp
HEADERS += ../vartypes/primitives/VarList.h

SOURCES += ../vartypes/primitives/VarStringEnum.cpp
HEADERS += ../vartypes/primitives/VarStringEnum.h

SOURCES += ../vartypes/primitives/VarSelection.cpp
HEADERS += ../vartypes/primitives/VarSelection.h

SOURCES += ../vartypes/primitives/VarExternal.cpp
HEADERS += ../vartypes/primitives/VarExternal.h

SOURCES += ../vartypes/primitives/VarString.cpp
HEADERS += ../vartypes/primitives/VarString.h

SOURCES += ../vartypes/primitives/VarStringVal.cpp
HEADERS += ../vartypes/primitives/VarStringVal.h

SOURCES += ../vartypes/primitives/VarBlob.cpp
HEADERS += ../vartypes/primitives/VarBlob.h

SOURCES += ../vartypes/primitives/VarQWidget.cpp
HEADERS += ../vartypes/primitives/VarQWidget.h

SOURCES += ../vartypes/primitives/VarTrigger.cpp
HEADERS += ../vartypes/primitives/VarTrigger.h

SOURCES += ../vartypes/xml/xmlParser.cpp
HEADERS += ../vartypes/xml/xmlParser.h

SOURCES += ../vartypes/gui/VarTreeView.cpp
HEADERS += ../vartypes/gui/VarTreeView.h

SOURCES += ../vartypes/gui/VarItem.cpp
HEADERS += ../vartypes/gui/VarItem.h

SOURCES += ../vartypes/gui/VarItemDelegate.cpp
HEADERS += ../vartypes/gui/VarItemDelegate.h

SOURCES += ../vartypes/gui/VarTreeModel.cpp
HEADERS += ../vartypes/gui/VarTreeModel.h

SOURCES += ../vartypes/gui/VarTreeViewOptions.cpp
HEADERS += ../vartypes/gui/VarTreeViewOptions.h

#qt-designed based UI-forms:
#none
#otherwise, use something like
#FORMS += src/mainwindow.ui

#build example executive
TARGET = example

CONFIG += qt console

#disable debugging for now...
#(debugging only works if qt-debug symbols are available)
CONFIG -= debug

#optional: enable OpenGL support
#QT     += opengl

#optional: enable networking
#QT	   += network

#ensure everything is placed in the 'src' folder:
UI_HEADERS_DIR = src
UI_SOURCES_DIR = src

RESOURCES += ../vartypes/gui/icons/icons.qrc

INCLUDEPATH += ../vartypes

win32 {
	CONFIG+=windows
	DEFINES+=WIN32
}

unix {
	DEFINES+=UNIX
}


