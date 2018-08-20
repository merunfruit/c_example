#include "beatbox.h"

int main() {
	printf("Beginning of beatbox program..\n");

	// Setup program environment
	AudioMixer_init();
	setupGPIO();
	i2cFileDesc = initI2CBus(I2CDRV_LINUX_BUS1, ACCELEROMETER_ADDRESS);
	setupAccelerometer(i2cFileDesc);
	volume = AudioMixer_getVolume();

	// Load drum sounds
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_BASE, &base);
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_HI_HAT, &hihat);
	AudioMixer_readWaveFileIntoMemory(SOURCE_FILE_SNARE, &snare);

	// Create threads
	pthread_attr_t attr;
	pthread_t* tid = (pthread_t*)malloc(sizeof(*tid)*NUM_THREAD);
	pthread_attr_init(&attr);

	pthread_create(&tid[0], &attr, playAudio, NULL); 			// Audio Thread
	pthread_create(&tid[1], &attr, readGPIO, NULL); 			// GPIO Thread
	pthread_create(&tid[2], &attr, readAccelerometer, NULL); 	// Accelerometer Thread
	pthread_create(&tid[3], &attr, listener, NULL); 			// Server Thread

	// Wait for threads to finish
	for(int i = 0; i < NUM_THREAD; i++) {
		pthread_join(tid[i], NULL);
	}

	// Cleanup program
	AudioMixer_cleanup();
	AudioMixer_freeWaveFileData(&base);
	AudioMixer_freeWaveFileData(&hihat);
	AudioMixer_freeWaveFileData(&snare);
	free(tid);
	close(i2cFileDesc);

	return EXIT_SUCCESS;
}

void* listener() {
		char message[MSG_MAX_LEN];

		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_port = htons(PORT);

		int socketDesc = socket(PF_INET, SOCK_DGRAM, 0);

		bind(socketDesc, (struct sockaddr*) &sin, sizeof(sin));

		while (!stopping) {
			unsigned int sin_len = sizeof(sin);
			int bytesRx = recvfrom(socketDesc, message, MSG_MAX_LEN, 0, (struct sockaddr *) &sin, &sin_len);

			message[bytesRx] = 0;
//			printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);

			returnPacket(message);

			sin_len = sizeof(sin);
			sendto(socketDesc, message, strlen(message), 0, (struct sockaddr *) &sin, sin_len);
		}

		close(socketDesc);
		puts("Server thread exiting...");
		pthread_exit(0);
}

void* readAccelerometer() {
	static int xCount = 0;
	static int yCount = 0;
	static int zCount = 0;

	while(!stopping) {
		accel_t res = readAccelerometerValue();

		int16_t x = res.x;
		int16_t y = res.y;
		int16_t z = res.z;

		// Z
		if(z < Z_THRESHOLD) {
			pthread_mutex_lock(&lock);
			zCount++;
			if(zCount == Z_MAX_COUNT) {
				AudioMixer_queueSound(&base);
				nanosleep(&accelDebounce, (struct timespec *) NULL);
				zCount = 0;
			}
			pthread_mutex_unlock(&lock);
		} else {
			pthread_mutex_lock(&lock);
			zCount = 0;
			pthread_mutex_unlock(&lock);
		}

		// Y
		if(y > XY_THRESHOLD) {
			pthread_mutex_lock(&lock);
			yCount++;
			if(yCount == XY_MAX_COUNT) {
				AudioMixer_queueSound(&hihat);
				nanosleep(&accelDebounce, (struct timespec *) NULL);
				yCount = 0;
			}
			pthread_mutex_unlock(&lock);
		} else {
			pthread_mutex_lock(&lock);
			yCount = 0;
			pthread_mutex_unlock(&lock);
		}

		// X
		if(x > XY_THRESHOLD) {
			pthread_mutex_lock(&lock);
			xCount++;
			if(xCount == XY_MAX_COUNT) {
				AudioMixer_queueSound(&snare);
				nanosleep(&accelDebounce, (struct timespec *) NULL);
				xCount = 0;
			}
			pthread_mutex_unlock(&lock);
		} else {
			pthread_mutex_lock(&lock);
			xCount = 0;
			pthread_mutex_unlock(&lock);
		}

//		printf("X IS: %4d | Y IS: %4d | Z IS %4d\n", res.x, res.y, res.z);
		nanosleep(&accelDebounce, (struct timespec *) NULL);
	}

	puts("Accelerometer thread exiting...");
	pthread_exit(0);
}

void changeVolume(int vol) {
	if(vol > MAX_VOLUME) vol = MAX_VOLUME;
	if(vol < MIN_VOLUME) vol = MIN_VOLUME;
	AudioMixer_setVolume(vol);
}

void* readGPIO() {
	while(!stopping) {
		_Bool js_up = checkJoystickUpValues();
		_Bool js_down = checkJoystickDownValues();
		_Bool js_left = checkJoystickLeftValues();
		_Bool js_right = checkJoystickRightValues();
		_Bool js_push = checkJoystickPushValues();

		if(js_up) {
			pthread_mutex_lock(&lock);
			volume += 5;

			changeVolume(volume);
			pthread_mutex_unlock(&lock);
		}
		if(js_down) {
			pthread_mutex_lock(&lock);
			volume -= 5;

			changeVolume(volume);
			pthread_mutex_unlock(&lock);
		}
		if(js_left) {
			pthread_mutex_lock(&lock);
			bpm -= 5;
			if(bpm < MIN_BPM) bpm = MIN_BPM;
			pthread_mutex_unlock(&lock);
		}
		if(js_right) {
			pthread_mutex_lock(&lock);
			bpm += 5;
			if(bpm > MAX_BPM) bpm = MAX_BPM;
			pthread_mutex_unlock(&lock);
		}
		if(js_push) {
			pthread_mutex_lock(&lock);
			changeTrack = true;
			pthread_mutex_unlock(&lock);
		}

		// Debounce
		nanosleep(&debounce, (struct timespec *) NULL);
	}

	puts("GPIO thread exiting...");
	pthread_exit(0);
}

void* playAudio() {
	while(!stopping) {
		if(track == 0) {
			playNoBeat();
		}
		else if(track == 1) {
			playRockBeat();
		}
		else if(track == 2) {
			playRandomBeat();
		}

		if(changeTrack) {
			pthread_mutex_lock(&lock);
			track = (track + 1) % 3;
			changeTrack = false;
			pthread_mutex_unlock(&lock);
		}

		// Debounce
		nanosleep(&debounce, (struct timespec *) NULL);
	}
	puts("Audio thread exiting...");
	pthread_exit(0);
}

void playRockBeat() {
//	printf("Playing Rock Beat...\n");
	while(track == 1 && !changeTrack && !stopping) {
		// Calculate delay
		int quarterBeat = (MINUTE_IN_MICRO / bpm / 4) * 1000;
		long nanoseconds = quarterBeat;
		struct timespec reqDelay = {DEBOUNCE_SECONDS, nanoseconds};

		// Play beat
		AudioMixer_queueSound(&base);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&snare);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
	}
}

void playNoBeat() {
//	printf("Playing No Beat...\n");
	volatile int count = 0;
	while(track == 0 && !changeTrack && !stopping) {
		count++;
	}
}

void playRandomBeat() {
//	printf("Playing Random Beat...\n");
	while(track == 2 && !changeTrack && !stopping) {
		// Calculate
		int quarterBeat = (MINUTE_IN_MICRO / bpm / 4) * 1000;
		long nanoseconds = quarterBeat;
		struct timespec reqDelay = {DEBOUNCE_SECONDS, nanoseconds};

		// Play beat
		AudioMixer_queueSound(&base);
		AudioMixer_queueSound(&snare);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&snare);
		AudioMixer_queueSound(&hihat);
		nanosleep(&reqDelay, (struct timespec *) NULL);
		AudioMixer_queueSound(&base);
		nanosleep(&reqDelay, (struct timespec *) NULL);
	}
}

