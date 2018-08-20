#ifndef BEATBOX_H_
#define BEATBOX_H_

#include "AudioMixer.h"
#include "JoystickController.h"
#include "I2CController.h"
#include "AccelerometerController.h"
#include "BeatboxServer.h"

#include <alsa/asoundlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#define SOURCE_FILE_BASE "beatbox-wav-files/100051__menegass__gui-drum-bd-hard.wav"
#define SOURCE_FILE_HI_HAT "beatbox-wav-files/100053__menegass__gui-drum-cc.wav"
#define SOURCE_FILE_SNARE "beatbox-wav-files/100059__menegass__gui-drum-snare-soft.wav"
#define MINUTE_IN_MICRO 60000000
#define DEBOUNCE_SECONDS 0
#define DEBOUNCE_NANOSECONDS 100000000
#define ACCELDEBOUNCE_NANOSECONDS 15000000
#define XY_THRESHOLD 3200
#define Z_THRESHOLD 800
#define XY_MAX_COUNT 7
#define Z_MAX_COUNT 5
#define MAX_VOLUME 100
#define MIN_VOLUME 0
#define MAX_BPM 300
#define MIN_BPM 40
#define NUM_THREAD 4

void* listener();
void* readAccelerometer();
void* playAudio();
void* readGPIO();
void changeVolume(int vol);
void playRockBeat();
void playNoBeat();
void playRandomBeat();

static _Bool changeTrack = false;

int i2cFileDesc;
int track = 0;
int volume = 0;
int bpm = 120;
_Bool stopping = false;
wavedata_t base;
wavedata_t hihat;
wavedata_t snare;

static struct timespec debounce = {DEBOUNCE_SECONDS, DEBOUNCE_NANOSECONDS};
static struct timespec accelDebounce = {DEBOUNCE_SECONDS, ACCELDEBOUNCE_NANOSECONDS};
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


#endif /* BEATBOX_H_ */
