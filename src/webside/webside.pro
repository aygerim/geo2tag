######################################################################
# Automatically generated by qmake (2.01a) Tue Jul 13 15:38:09 2010
######################################################################

TEMPLATE = app
include(../../config.pri)
TARGET = sketch.wt
DEPENDPATH += . inc src
INCLUDEPATH += . inc
QMAKE_CXXFLAGS += -g3
QT+= network
config += network
CONFIG += no_keywords
# Wt
INCLUDEPATH += /usr/include/Wt
LIBS += -lwt -lwtfcgi -lcommon -lwikigpsHttp -lwikigpsJson -lboost_signals 


# Common library
INCLUDEPATH += ../common/inc \
               ../common/common/inc \
               ../common/thread/inc \
               ../common/exception/inc \
	       ../http_requests/inc \
               ../common/odbc/inc \
	       ../json/inc 



HEADERS += inc/LoginWidget.h \
	   inc/Webside.h \ 
	   inc/qt_signals_fix.h
#    inc/UsersWidget.h

SOURCES += src/LoginWidget.cpp \
	   src/Webside.cpp 
#    src/UsersWidget.cpp

# Input
HEADERS += inc/LoginWidgetConnector.h \
	   inc/MarksModelConnector.h \
	   inc/MarksWidget.h \
           inc/MarksModel.h \
           inc/sketch.h \ 
           inc/GoogleMap.h \
           inc/OpenStreetMap.h \
           inc/YandexMap.h 

#	   inc/ChannelsWidget.h \
#	   inc/ChannelsModel.h \
#          inc/OptionsModel.h \
#          inc/OptionsWidget.h \
#          inc/PrefWidget.h 
SOURCES += src/main.cpp \
           src/MarksModel.cpp \
           src/MarksWidget.cpp \
           src/sketch.cpp \
           src/GoogleMap.cpp \
           src/OpenStreetMap.cpp \
	   src/LoginWidgetConnector.cpp \
	   src/MarksModelConnector.cpp \
           src/YandexMap.cpp
#	   src/ChannelsWidget.cpp \
#	   src/ChannelsModel.cpp \
#          src/OptionsModel.cpp \
#          src/OptionsWidget.cpp \
#          src/PrefWidget.cpp 
