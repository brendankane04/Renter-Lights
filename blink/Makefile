#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := blink

include $(IDF_PATH)/make/project.mk

setup: 
	%userprofile%\OneDrive\Desktop\esp-idf\export.bat

flash: 
	idf.py build && idf.py -p COM6 flash
	
test:
	echo "TEST"