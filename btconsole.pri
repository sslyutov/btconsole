# Bluetooth LE Central 
# btprofile.pri
# date August 31, 2022
# author Sergey Slyutov


isEmpty(SRCROOT) {
	SRCROOT = "./src"
}

INCLUDEPATH += $${SRCROOT}




SOURCES += $${SRCROOT}/main.cpp      

#HEADERS += $${SRCROOT}/btmon.h

# FORMS	+= $${SRCROOT}/btmon.ui