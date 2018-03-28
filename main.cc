#include <stdio.h>
#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <alsa/asoundlib.h>
#define DESIRED_RATE 74500

int main(int argc, char *argv[]){
	int i;
	unsigned int myRate = DESIRED_RATE;
	int err;
	
	double x, cost;
	double freq = 600;	

	short buf[200];
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params;
	
	/*Open Sound Device*/
	snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);

	/*Allocate Hardware Parameters structures and fills it with 
		config space for PCM*/
	snd_pcm_hw_params_malloc(&hw_params);
	snd_pcm_hw_params_any(playback_handle, hw_params);

	/*Set Parameters : interleaved channel,
				16 bits little endian
				44100Hz
				2 channels*/
	snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near (playback_handle, hw_params, &myRate, 0);
	snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2);

	/*Assign them to the playback_handle and free the param stucture*/
	snd_pcm_hw_params(playback_handle, hw_params);
	snd_pcm_hw_params_free(hw_params);

	/*prep and play*/
	snd_pcm_prepare(playback_handle);
	
	for(int i=0; i<200; i++){
		buf[i] = sin(2*3.14*freq/myRate*i);
	}

	for(int i=0; i<7000; i++){
		if(snd_pcm_writei(playback_handle, buf, 200) < 200){
			printf("caught seg fault!\n");
		}
	}
	snd_pcm_close(playback_handle);
	return 0;
}
