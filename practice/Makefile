TARGET_NAME	= gino
TARGET = ../bin/$(TARGET_NAME)
all:gino 
CROSS_COMPILE	?= ../../../buildroot-2014.11/bin/arm-buildroot-linux-uclibcgnueabi-
cc = $(CROSS_COMPILE)gcc
OBJECTS = test.o fun.o
gino:$(OBJECTS)
	$(cc) -o $(TARGET) $(OBJECTS)

.c.o:
	$(cc) -c -o $@ $<
	
clean:
	rm gino *.o



	


#@$(CC) -o $(TARGET) $(OBJECTS) $(LIBS) $(LDFLAGS) $(LDDEPS)
#include ../config.mk 


#@$(CC) -o $(TARGET) $(OBJECTS) $(LIBS) $(LDFLAGS) $(LDDEPS)
#include ../config.mk 
