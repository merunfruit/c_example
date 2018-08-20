CC_FLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror
CROSS_TOOL = arm-linux-gnueabihf-
CC_CPP = $(CROSS_TOOL)g++
CC_C = $(CROSS_TOOL)gcc
SRC = beatbox.c
TARGET = beatbox
PUBDIR = $(HOME)/cmpt433/public/myApps

LFLAGS = -L$(HOME)/cmpt433/public/asound_lib_BBB

help:
	echo "Targets are: all, app, wav, node"

all: app wav node

app: AudioMixer.o JoystickController.o I2CController.o AccelerometerController.o BeatboxServer.o
	$(CC_C) $(CC_FLAGS) AudioMixer.o JoystickController.o I2CController.o AccelerometerController.o BeatboxServer.o $(SRC) -o $(TARGET) $(LFLAGS) -lpthread -lasound
	cp $(TARGET) $(PUBDIR)/$(TARGET)

AudioMixer.o: AudioMixer.c
	$(CC_C) $(CC_FLAGS) -c AudioMixer.c
	
JoystickController.o: JoystickController.c
	$(CC_C) $(CC_FLAGS) -c JoystickController.c
	
I2CController.o: I2CController.c
	$(CC_C) $(CC_FLAGS) -c I2CController.c

AccelerometerController.o: AccelerometerController.c
	$(CC_C) $(CC_FLAGS) -c AccelerometerController.c

BeatboxServer.o: BeatboxServer.c
	$(CC_C) $(CC_FLAGS) -c BeatboxServer.c

wav:
	mkdir -p $(PUBDIR)/beatbox-wav-files/
	cp -R beatbox-wav-files/* $(PUBDIR)/beatbox-wav-files/

node:
	mkdir -p $(PUBDIR)/beatbox-server-copy/
	cp -R as3-server/* $(PUBDIR)/beatbox-server-copy/

clean:
	rm -f *o $(PUBDIR)/beatbox