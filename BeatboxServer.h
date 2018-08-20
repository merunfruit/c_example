#ifndef BEATBOXSERVER_H_
#define BEATBOXSERVER_H_

#include "AudioMixer.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <netdb.h>
#include <string.h>

#define PROC_UPTIME_PATH "/proc/uptime"
#define NONE "None"
#define ROCK "Rock"
#define CUSTOM "Custom"
#define MSG_MAX_LEN 1024
#define PORT 12345
#define MAX_BPM 300
#define MIN_BPM 40
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define COMMAND_TRACK "track"
#define COMMAND_MODE "mode"
#define COMMAND_VOL "volume"
#define COMMAND_BPM "bpm"
#define COMMAND_SOUND "sound"
#define COMMAND_UPTIME "uptime"
#define COMMAND_UNKNOWN "unknown"

extern wavedata_t base;
extern wavedata_t snare;
extern wavedata_t hihat;
extern _Bool stopping;
extern int track;
extern int bpm;

typedef struct {
	int hour;
	int minute;
	int second;
} uptime_t;

uptime_t getUpTime();
int convertToInt(char *string);
void returnPacket(char* message);

#endif /* BEATBOXSERVER_H_ */
