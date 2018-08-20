#include "BeatboxServer.h"

void returnPacket(char *command) {
	if(strncmp(command, COMMAND_TRACK, strlen(COMMAND_TRACK)) == 0) {
		int trackNum = convertToInt(command);

		sprintf(command, "Changing track to %d\n", trackNum);
		track = trackNum;
	}

	else if(strncmp(command, COMMAND_MODE, strlen(COMMAND_MODE)) == 0) {
		if(track == 0) {
			sprintf(command, "mode %s", NONE);
		}
		else if(track == 1) {
			sprintf(command, "mode %s", ROCK);
		}
		else {
			sprintf(command, "mode %s", CUSTOM);
		}
	}

	else if(strncmp(command, COMMAND_VOL, strlen(COMMAND_VOL)) == 0) {
		int vol = convertToInt(command);

		vol += AudioMixer_getVolume();
		AudioMixer_changeVolume(vol);
		vol = AudioMixer_getVolume();
		sprintf(command, "volume %d", vol);

	}

	else if(strncmp(command, COMMAND_BPM, strlen(COMMAND_BPM)) == 0) {
		int newBPM = convertToInt(command);

		newBPM += bpm;
		if(newBPM > MAX_BPM) newBPM = MAX_BPM;
		if(newBPM < MIN_BPM) newBPM = MIN_BPM;
		bpm = newBPM;
		sprintf(command, "bpm %d", newBPM);

	}

	else if(strncmp(command, COMMAND_SOUND, strlen(COMMAND_SOUND)) == 0) {
		int sound = convertToInt(command);

		sprintf(command, "Playing sound %d\n", sound);
		if(sound == 0) {
			AudioMixer_queueSound(&base);
		}
		else if(sound == 1) {
			AudioMixer_queueSound(&hihat);
		}
		else if(sound == 2) {
			AudioMixer_queueSound(&snare);
		}
	}

	else if(strncmp(command, COMMAND_UPTIME, strlen(COMMAND_UPTIME)) == 0) {
		uptime_t uptime = getUpTime();
		sprintf(command, "Uptime is %dH %dM %dS", uptime.hour, uptime.minute, uptime.second);
	}

	else {
		sprintf(command, "Unknown command\n");
	}
}

uptime_t getUpTime() {
	uptime_t uptime;

	FILE *pfile = fopen(PROC_UPTIME_PATH, "r");
	if (pfile == NULL) {
		printf("Unable to open uptime file.\n");
	}

	//Read file
	int buff = 0;
	fscanf(pfile, "%d", &buff);
	fclose(pfile);

	// Calculate time
	int hour = buff / SECONDS_IN_HOUR;
	buff %= SECONDS_IN_HOUR;
	int minute = buff / SECONDS_IN_MINUTE;
	buff %= SECONDS_IN_MINUTE;

	uptime.hour = hour;
	uptime.minute = minute;
	uptime.second = buff;

	// 1 for not press, 0 for pressed
	return uptime;
}

int convertToInt(char *string) {
	int value = -1;
	sscanf(string, "%*s%d", &value);
	return value;
}
